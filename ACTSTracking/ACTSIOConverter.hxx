#ifndef ACTSIOConverter_h
#define ACTSIOConverter_h 1

// Gaudi
#include <GaudiAlg/GaudiAlgorithm.h>
#include <GaudiAlg/Transformer.h>
#include <k4FWCore/BaseClass.h>

// k4FWCore
#include <k4FWCore/DataHandle.h>

#include <any>

//! \brief Converts between LCIO and EDM4HEP
/**
 * @author Samuel Ferraro
 * @version $Id$
 */
struct ACTSIOConverter final : Gaudi::Functional::Transformer <std::any(const std::any&)> {
public:
	/**
         * @brief Constructor for ACTSIOConverter
         * @param name unique string identifier for this instance
         * @param svcLoc a Service Locator passed by the Gaudi AlgManager
         */
	ACTSIOConverter(const std::string& name, ISvcLocator* svcLoc);
	
	/**
         * @brief ACTSIOConverter operation. The workhorse of this Transformer.
         * @param any A collection LCIO or EDM4HEP
         * @return any A collection LCIO or EDM4HEP
         */
	std::any operator()(const std::any& collection) const override;

protected:
	Gaudi::Property<bool> m_L2E{this, "LCIO2EDM4HEP", true, "Determines the direction of conversion."};
};

#endif
