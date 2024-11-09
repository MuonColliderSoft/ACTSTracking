#include "ACTSIOConverter.hxx"
#include <k4EDM4hep2LcioConv.h>
#include <k4Lcio2EDM4hepConv.h>


DECLARE_COMPONENT(ACTSIOConverter)

ACTSIOConverter::ACTSIOConverter(const std::string& name, ISvcLocator* svcLoc) : Transformer(name, svcLoc,
				KeyValue("InputCollectionName", "InputCollection"),
				KeyValue("OutputCollectionName", "OutputCollection")) {}

std::any ACTSIOConverter::operator()(const std::any& inputCollection) const{
	MsgStream log(msgSvc(), name());
	if (m_L2E) {
		

	} else {
		
	}
}
