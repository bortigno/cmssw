#ifndef EMTFUnpacker__h
#define EMTFUnpacker__h


#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Utilities/interface/InputTag.h>

//CSC Track Finder Raw Data Format
#include "EventFilter/CSCTFRawToDigi/src/CSCTFEvent.h"

//FEDRawData
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

#include <vector>
#include <string>


class EMTFUnpacker: public edm::stream::EDProducer<> {
private:

	// label of the module which produced raw data
	edm::InputTag producer;

	edm::EDGetTokenT<FEDRawDataCollection> Raw_token;

    void beginJob();
	void produce(edm::Event& e, const edm::EventSetup& c);
	void endJob();

public:

	explicit EMTFUnpacker(const edm::ParameterSet& pset);
	~EMTFUnpacker(void);

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};

#endif


// //
// // class declaration
// //

// class EMTFUnpacker : public edm::EDProducer {
//    public:
//       explicit EMTFUnpacker(const edm::ParameterSet&);
//       ~EMTFUnpacker();

//       static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

//    private:
//       virtual void beginJob() override;
//       virtual void produce(edm::Event&, const edm::EventSetup&) override;
//       virtual void endJob() override;
      
//       //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
//       //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
//       //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
//       //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

//       // ----------member data ---------------------------
// };
