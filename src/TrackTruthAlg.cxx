#include "TrackTruthAlg.hxx"
#include "Helpers.hxx"

#include <edm4hep/MCParticle.h>
#include <edm4hep/SimTrackerHit.h>
#include <edm4hep/Track.h>
#include <edm4hep/TrackerHit.h>
#include <edm4hep/MCRecoTrackerAssociation.h>



//------------------------------------------------------------------------------------------------

DECLARE_COMPONENT(TrackTruthAlg)

TrackTruthAlg::TrackTruthAlg(const std::string& name, ISvcLocator* svcLoc) : MultiTransformer(name, svcLoc, {
		KeyValue("InputTrackCollectionName", "Tracks"),
		KeyValue("InputMCParticleCollectionName", "MCParticle"),
		KeyValue("InputTrackerHit2SimTrackerHitRelationName", "TrackMCRelation") },
		{ KeyValue("OutputParticle2TrackRelationName", "Particle2TrackRelationName") })	{}

std::tuple<edm4hep::MCRecoTrackParticleAssociationCollection> TrackTruthAlg::operator()(
			const edm4hep::TrackCollection& tracks,
                        const edm4hep::MCParticleCollection& mcParticles,
                        const edm4hep::MCRecoTrackerAssociationCollection& trackerHitRelations) const{
	// Map TrackerHits to SimTrackerHits
	std::map<edm4hep::TrackerHit, edm4hep::SimTrackerHit> trackerHit2SimHit;
	for (const auto& hitRel : trackerHitRelations) {
		auto trackerHit = hitRel.getRec();
		auto simTrackerHit = hitRel.getSim();
		trackerHit2SimHit[trackerHit] = simTrackerHit;
	}

	// Map best matches MCP to Track
	std::map<edm4hep::MCParticle, edm4hep::Track> mcBestMatchTrack;
	std::map<edm4hep::MCParticle, float> mcBestMatchFrac;

	for (const auto& track: tracks) {
		//Get Track
		std::map<edm4hep::MCParticle, uint32_t> trackHit2Mc;
		for (auto& hit : track.getTrackerHits()) {
			//Search for SimHit
			edm4hep::SimTrackerHit* simHit = nullptr;
			const auto simHitIter = trackerHit2SimHit.find(hit);
			if (simHitIter != trackerHit2SimHit.end()) { //Found SimHit
				auto simHit = simHitIter->second;
				break;
			}
			if (simHit->getMCParticle().isAvailable()) {
				trackHit2Mc[simHit->getMCParticle()]++; //Increment MC Particle counter
			}
		}

		// Update Best Matches
		for (const auto& [mcParticle, hitCount] : trackHit2Mc) {
			float frac = static_cast<float>(hitCount) / track.trackerHits_size();
			bool better = mcBestMatchTrack.count(mcParticle) == 0 || // no best matches exist
				      mcBestMatchFrac[mcParticle] < frac; // this match is better (more hits on track)
			if (better) {
				mcBestMatchTrack[mcParticle] = track;
				mcBestMatchFrac[mcParticle] = frac;
			}
		}
	}

	// Save the best matches
	edm4hep::MCRecoTrackParticleAssociationCollection outColMC2T;
	for (const auto& [mcParticle, track] : mcBestMatchTrack) {
		edm4hep::MutableMCRecoTrackParticleAssociation association = outColMC2T.create();
		association.setRec(track);
		association.setSim(mcParticle);
		association.setWeight(mcBestMatchFrac[mcParticle]);
	}

	return std::make_tuple(std::move(outColMC2T));
}