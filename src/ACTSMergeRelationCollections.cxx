#include "ACTSMergeRelationCollections.hxx"

// edm4hep
#include <edm4hep/MCRecoTrackerHitPlaneAssociation.h>


DECLARE_COMPONENT(ACTSMergeRelationCollections)


ACTSMergeRelationCollections::ACTSMergeRelationCollections(const std::string& name, ISvcLocator* svcLoc) : MultiTransformer(name, svcLoc, {
		KeyValue("InputCollection1", "Collection1"),
		KeyValue("InputCollection2", "Collection2"),
		KeyValue("InputCollection3", "Collection3"),
		KeyValue("InputCollection4", "Collection4"),
		KeyValue("InputCollection5", "Collection5"),
		KeyValue("InputCollection6", "Collection6") },
	      { KeyValue("OutputCollection", "MergedCollection") }) {}

std::tuple<edm4hep::MCRecoTrackerHitPlaneAssociationCollection> ACTSMergeRelationCollections::operator()(
		const DataWrapper<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>& col1,
		const DataWrapper<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>& col2,
		const DataWrapper<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>& col3,
		const DataWrapper<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>& col4,
		const DataWrapper<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>& col5,
		const DataWrapper<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>& col6) const{
	// Initialize new collection
	edm4hep::MCRecoTrackerHitPlaneAssociationCollection mergedCollection;

	mergedCollection.setSubsetCollection();

	// Loop over all item in all collections and add them to the new collection
	for (const auto& col : {col1.getData(), col2.getData(), col3.getData(), col4.getData(), col5.getData(), col6.getData()}) {
		for (const auto& item : *col) {
			mergedCollection.push_back(item);
		}
	}

	return std::make_tuple(std::move(mergedCollection));
}
