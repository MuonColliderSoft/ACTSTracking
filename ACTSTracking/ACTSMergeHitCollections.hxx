#ifndef ACTSMergeHitCollections_h
#define ACTSMergeHitCollections_h 1

// edm4hep
#include <edm4hep/TrackerHitPlaneCollection.h>

// k4FWCore
#include <k4FWCore/DataHandle.h>
#include <k4FWCore/Transformer.h>

// Standard
#include <vector>

/**
 * @brief Combine 6 TrackerHitPlaneCollections collections into 1
 * @TODO: I know for a fact that this and ACTSMergeRelationCollections can be the same class. I just cannot get the inhertance structure of edm4hep to work for me...
 * @author Samuel Ferraro
 * @version $Id$
 */
struct ACTSMergeHitCollections final : k4FWCore::Transformer<edm4hep::TrackerHitPlaneCollection(const std::vector<const edm4hep::TrackerHitPlaneCollection*>&)> {
public:
	/**
         * @brief Constructor for ACTSMergeHitCollections
         * @param name unique string identifier for this instance
         * @param svcLoc a Service Locator passed by the Gaudi AlgManager
         */
	ACTSMergeHitCollections(const std::string& name, ISvcLocator* svcLoc);

	/**
         * @brief ACTSMergeHitCollection operation. The workhorse of this MultiTransformer.
         * @param cols A vector collection of tracker hits from one section of the detector
         * @return A merged collection with all tracker hits.
         */
	edm4hep::TrackerHitPlaneCollection operator()(
		const std::vector<const edm4hep::TrackerHitPlaneCollection*>& cols) const override;
};
#endif // ACTSMergeHitCollections_h
