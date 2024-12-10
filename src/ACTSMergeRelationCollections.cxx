#include "ACTSMergeRelationCollections.hxx"

// edm4hep
#include <edm4hep/TrackerHitSimTrackerHitLink.h>


DECLARE_COMPONENT(ACTSMergeRelationCollections)


ACTSMergeRelationCollections::ACTSMergeRelationCollections(const std::string& name, ISvcLocator* svcLoc) : MultiTransformer(name, svcLoc, {
		KeyValues("InputCollection1", {"Collection1"}),
		KeyValues("InputCollection2", {"Collection2"}),
		KeyValues("InputCollection3", {"Collection3"}),
		KeyValues("InputCollection4", {"Collection4"}),
		KeyValues("InputCollection5", {"Collection5"}),
		KeyValues("InputCollection6", {"Collection6"}) },
	      { KeyValues("OutputCollection", {"MergedCollection"}) }) {}

std::tuple<edm4hep::TrackerHitSimTrackerHitLinkCollection> ACTSMergeRelationCollections::operator()(
		const edm4hep::TrackerHitSimTrackerHitLinkCollection& col1,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection& col2,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection& col3,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection& col4,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection& col5,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection& col6) const{
	// Initialize new collection
	edm4hep::TrackerHitSimTrackerHitLinkCollection mergedCollection;

	mergedCollection.setSubsetCollection();

	// Loop over all item in all collections and add them to the new collection
	std::array<const edm4hep::TrackerHitSimTrackerHitLinkCollection*, 6> collections = {&col1, &col2, &col3, &col4, &col5, &col6};

        for (auto* col : collections) {
		for (const auto& item : *col) {
			mergedCollection.push_back(item);
		}
	}

	return std::make_tuple(std::move(mergedCollection));
}
