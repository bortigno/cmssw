// -*- C++ -*-
//
// Package:    EventFilter/EMTFUnpacker
// Class:      EMTFUnpacker
// 
/**\class EMTFUnpacker EMTFUnpacker.cc EventFilter/EMTFRawToDigi/plugins/EMTFUnpacker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Pierluigi Bortignon
//         Created:  Thu, 12 Nov 2015 00:42:22 GMT
//
//

// system include files
#include <memory>

#include <stdio.h>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include <FWCore/Utilities/interface/InputTag.h>


//FEDRawData
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

//Digi collections
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1CSCTrackCollection.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1CSCStatusDigiCollection.h"
#include "DataFormats/L1CSCTrackFinder/interface/CSCTriggerContainer.h"

#include <iostream>

#include "EventFilter/EMTFRawToDigi/interface/EMTFUnpacker.h"


//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
EMTFUnpacker::EMTFUnpacker(const edm::ParameterSet& iConfig)
{
   //register your products
   produces<L1CSCTrackCollection>();

/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
 
   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   //now do what ever other initialization is needed

  producer = iConfig.getParameter<edm::InputTag>("producer");
  Raw_token = consumes<FEDRawDataCollection>(edm::InputTag(producer.label(),producer.instance() ));


}


EMTFUnpacker::~EMTFUnpacker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EMTFUnpacker::produce(edm::Event& e, const edm::EventSetup& c)
{
   using namespace edm;
/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::unique_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(std::move(pOut));
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/

    // Get a handle on the FED data collection
    edm::Handle<FEDRawDataCollection> rawdata;
    e.getByToken(Raw_token,rawdata);


    // looking at https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/EventFilter/CSCTFRawToDigi/interface/CSCTFUnpacker.h
    // and https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/EventFilter/CSCTFRawToDigi/plugins/CSCTFUnpacker.cc

    // create the collection of CSC wire and strip digis as well as of DT stubs, which we receive from DTTF
    // std::auto_ptr<CSCCorrelatedLCTDigiCollection> LCTProduct(new CSCCorrelatedLCTDigiCollection);
    // std::auto_ptr<L1CSCTrackCollection>           trackProduct(new L1CSCTrackCollection);
    // std::auto_ptr<L1CSCStatusDigiCollection>      statusProduct(new L1CSCStatusDigiCollection);
    // std::auto_ptr<CSCTriggerContainer<csctf::TrackStub> > dtProduct(new CSCTriggerContainer<csctf::TrackStub>);

    for(int fedid=1350; fedid<=1409; fedid++){
        const FEDRawData& fedData = rawdata->FEDData(fedid);
        if( fedData.size()==0 ) continue;
        std::cout << "fed id = " << fedid << std::endl;
        std::cout << "fedData size = " << fedData.size() << std::endl;
        //LogDebug("CSCTFUnpacker|produce");
        //if( monitor ) monitor->process((unsigned short*)fedData.data());
        //unsigned int unpacking_status = tfEvent.unpack((unsigned short*)fedData.data(),fedData.size()/2);
	}

}

// ------------ method called once each job just before starting event loop  ------------
void 
EMTFUnpacker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
EMTFUnpacker::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
EMTFUnpacker::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
EMTFUnpacker::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
EMTFUnpacker::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
EMTFUnpacker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EMTFUnpacker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

