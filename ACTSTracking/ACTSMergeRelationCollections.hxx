#ifndef ACTSMergeRelationCollections_h
#define ACTSMergeRelationCollections_h 1

// edm4hep
#include <edm4hep/TrackerHitSimTrackerHitLinkCollection.h>

// k4FWCore
#include <k4FWCore/DataHandle.h>
#include <k4FWCore/Transformer.h>

// Standard
#include <tuple>

/**
 * @brief Combine 6 TrackerHitSimTrackerHitLinkCollections collections into 1
 * @TODO: I know for a fact that this and ACTSMergeHitCollections can be the same class. I just cannot get the inhertance structure of edm4hep to work for me...
 * @author Samuel Ferraro
 * @version $Id$
 */
struct ACTSMergeRelationCollections final : k4FWCore::MultiTransformer<std::tuple<edm4hep::TrackerHitSimTrackerHitLinkCollection>(
		const edm4hep::TrackerHitSimTrackerHitLinkCollection &,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection &,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection &,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection &,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection &,
		const edm4hep::TrackerHitSimTrackerHitLinkCollection &)> {
public:
	/**
         * @brief Constructor for ACTSMergeRelationCollections
         * @param name unique string identifier for this instance
         * @param svcLoc a Service Locator passed by the Gaudi AlgManager
         */
	ACTSMergeRelationCollections(const std::string& name, ISvcLocator* svcLoc);

	/**
         * @brief ACTSMergeHitCollection operation. The workhorse of this MultiTransformer.
         * @param col A collection of tracker hit associations from one section of the detector
         * @return A merged collection with all tracker hits associations.
         */
	std::tuple<edm4hep::TrackerHitSimTrackerHitLinkCollection> operator()(
		const edm4hep::TrackerHitSimTrackerHitLinkCollection& col1,
                const edm4hep::TrackerHitSimTrackerHitLinkCollection& col2,
                const edm4hep::TrackerHitSimTrackerHitLinkCollection& col3,
                const edm4hep::TrackerHitSimTrackerHitLinkCollection& col4,
                const edm4hep::TrackerHitSimTrackerHitLinkCollection& col5,
                const edm4hep::TrackerHitSimTrackerHitLinkCollection& col6) const override;
};
#endif // ACTSMergeRelationCollections_h
