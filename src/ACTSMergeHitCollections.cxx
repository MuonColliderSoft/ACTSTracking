#include "ACTSMergeHitCollections.hxx"

// edm4hep
#include <edm4hep/TrackerHitPlane.h>


DECLARE_COMPONENT(ACTSMergeHitCollections)


ACTSMergeHitCollections::ACTSMergeHitCollections(const std::string& name, ISvcLocator* svcLoc) : Transformer(name, svcLoc, {
		KeyValues("InputCollections", {"Collection"}) },
	      { KeyValues("OutputCollection", {"MergedCollection"}) }) {}

edm4hep::TrackerHitPlaneCollection ACTSMergeHitCollections::operator()(
		const std::vector<const edm4hep::TrackerHitPlaneCollection*>& cols) const{
	// Initialize collection
	edm4hep::TrackerHitPlaneCollection mergedCollection;
	
	mergedCollection.setSubsetCollection();
	// Loop over all items in all collections and add each to a new collection
	for (size_t i = 0; i < cols.size(); ++i) {
		for (const auto& item : *(cols[i])) {
			mergedCollection.push_back(item);
		}
	}

	return mergedCollection;
}
