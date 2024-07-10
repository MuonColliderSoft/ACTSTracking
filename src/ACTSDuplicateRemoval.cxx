#include "ACTSDuplicateRemoval.hxx"

#include <edm4hep/TrackCollection.h>
#include <edm4hep/Track.h>
#include <edm4hep/TrackerHit.h>
#include <GaudiKernel/MsgStream.h>
#include <GaudiKernel/SmarkIF.h>

#include <algorithm>

namespace ACTSTracking {
/**
 * Return true if `trk1` and `trk2` share at least 50% of hits.
 */
inline bool tracks_equal(const edm4hep::Track& trk1, const edm4hep::Track& trk2) {
	const auto& hits1 = trk1.getTrackerHits();
	const auto& hits2 = trk2.getTrackerHits();

	// Number of overlapping hist
	uint32_t hitOlap = 0;
	for (const auto& hit1 : hits1) {
		if (std::find(hits2.begin(), hits2.end(), hit1) != hits2.end()) {
			hitOlap++;
		}
	}

	// Smaller track count
	uint32_t size = std::min(hits1.size(), hits2.size());

	return 2 * hitOlap > size;  // half of smaller track belong to larger track
}

/**
 * Return true if `trk1` is of higher quality than `trk2`.
 */
bool track_quality_compare(const edm4hep::Track& trk1, const edm4hep::Track& trk2) {
	// If number of hits are different, then the one with more
	// hits should be chosen first
	if (trk1.trackerHits_size() != trk2.trackerHits_size())
		return trk1.trackerHits_size() > trk2.trackerHits_size();

	// Same number of hits means I want smaller chi2
	return trk1.getChi2() < trk2.getChi2();
}

inline bool track_duplicate_compare(const edm4hep::Track& trk1, const edm4hep::Track& trk2) {
	return trk1.getTrackState(edm4hep::TrackState::AtIP).tanLambda < trk2.getTrackState(edm4hep::TrackState::AtIP).tanLambda;
}
}  // namespace ACTSTracking

DECLARE_COMPONENT(ACTSDuplicateRemoval)

ACTSDuplicateRemoval::ACTSDuplicateRemoval(const std::string& name, ISvcLocator* svcLoc) : GaudiAlgorithm(name, svcLoc) {
	declareProperty("InputTrackCollectionName", m_inputTrackCollection, "Name of input track collection", std::string("TruthTracks"));
	declareProperty("OutputTrackCollection", m_outputTrackCollection, "Name of output track collection", std::string("DedupedTruthTracks"));
}

StatusCode ACTSDuplicateRemoval::initialize() {
	info() << "Initializing ACTSDuplicateRemoval" << endmsg;
	return StatusCode::SUCCESS;
}

void ACTSDuplicateRemoval::execute() {
	const edm4hep::TrackCollection trackCollecion = nullptr;
	if (ACTSTracking::getCollecion(evtSvc(), m_inputTrackCollection, trackCollection).isFailure()) { return StatusCode::FAILURE; }
	auto outputTracks = new edm4hep::TrackCollection();

	std::vector<edm4hep::Track> sortedInput;
	for (const auto& track : *inputTracks) {
		auto insertion_point = std::upper_bound(sortedInput.begin(), sortedInput.end(), track, ACTSTracking::track_duplicate_compare);
		sortedInput.insert(insertion_point, track);
	}

	std::vector<edm4hep::Track> finalTracks;
	for (const auto& track : sortedInput) {
		bool foundAnEqual = false;
		for (int i = (finalTracks.size() >= 10) ? finalTracks.size() - 10: 0; i < finalTracks.size(); ++i) {
			const auto& otherTrack = finalTracks[i];
			if (!ACTSTracking::tracks_equal(track, otherTrack)) continue;
			foundAnEqual = true;
			if (ACTSTracking::track_quality_compare(track, otherTrack)) {
				finalTracks[i] = track;
				break;
			}
		}
		if (!foundAnEqual) {
			finalTracks.push_back(track);
		}
	}

	for (const auto& track : finalTracks) {
		outputTracks->push_back(track);
	}

	put(outputTracks, m_outputTrackCollection);

	return StatusCode::SUCCESS;
}

StatusCode ACTSDuplicateRemoval::finalize() {
	info() << "Finalizeing ACTSDuplicateRemoval" << endmsg;
	return StatusCode::SUCCESS;
}
