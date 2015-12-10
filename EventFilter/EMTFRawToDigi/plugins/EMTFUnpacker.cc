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


//MTF7 
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_header_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event_generator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_data_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_operator_builder.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/unity_test.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/block_operator.h"
#include <string>
#include <fstream>

#include "TTree.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

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
   ntuple  = new TFile("EMTF_ntuple.root","RECREATE");
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

        EMTF = new TTree("EMTF","EMTF");
	//****************************************************************************
	//                BEGIN CREATE NTUPLE                  
	//**************************************************************************** 
	//{{{
	EMTF->Branch("amc13_header_orn", &amc13_header_orn);
	EMTF->Branch("amc13_header_lv1_id", &amc13_header_lv1_id);
	EMTF->Branch("amc13_header_bx_id", &amc13_header_bx_id);
	EMTF->Branch("amc13_header_source_id", &amc13_header_source_id);
	EMTF->Branch("amc13_header_evt_ty", &amc13_header_evt_ty);
	EMTF->Branch("amc13_header_fov", &amc13_header_fov);
	EMTF->Branch("amc13_header_ufov", &amc13_header_ufov);
	EMTF->Branch("amc13_header_res", &amc13_header_res);
	EMTF->Branch("amc13_header_namc", &amc13_header_namc);
	EMTF->Branch("amc13_header_h", &amc13_header_h);
	EMTF->Branch("amc13_header_x", &amc13_header_x);
	EMTF->Branch("prepayload_l", &prepayload_l);
	EMTF->Branch("prepayload_m", &prepayload_m);
	EMTF->Branch("prepayload_s", &prepayload_s);
	EMTF->Branch("prepayload_e", &prepayload_e);
	EMTF->Branch("prepayload_p", &prepayload_p);
	EMTF->Branch("prepayload_v", &prepayload_v);
	EMTF->Branch("prepayload_c", &prepayload_c);
	EMTF->Branch("prepayload_amcno", &prepayload_amcno);
	EMTF->Branch("prepayload_blkNo", &prepayload_blkNo);
	EMTF->Branch("prepayload_amcn_size", &prepayload_amcn_size);
	EMTF->Branch("prepayload_boardid", &prepayload_boardid);
	EMTF->Branch("amc_header_amc_number", &amc_header_amc_number);
	EMTF->Branch("amc_header_bx_id", &amc_header_bx_id);
	EMTF->Branch("amc_header_orbit_number", &amc_header_orbit_number);
	EMTF->Branch("amc_header_board_id", &amc_header_board_id);
	EMTF->Branch("amc_header_lv1_id", &amc_header_lv1_id);
	EMTF->Branch("amc_header_data_length", &amc_header_data_length);
	EMTF->Branch("amc_header_user_id", &amc_header_user_id);
	EMTF->Branch("amc_trailer_lv1_id", &amc_trailer_lv1_id);
	EMTF->Branch("amc_trailer_crc32", &amc_trailer_crc32);
	EMTF->Branch("amc_trailer_data_length", &amc_trailer_data_length);
	EMTF->Branch("l1a", &l1a);
	EMTF->Branch("bxn", &bxn);
	EMTF->Branch("sp_ts", &sp_ts);
	EMTF->Branch("sp_ersv", &sp_ersv);
	EMTF->Branch("sp_addr", &sp_addr);
	EMTF->Branch("tbin", &tbin);
	EMTF->Branch("ddm", &ddm);
	EMTF->Branch("spa", &spa);
	EMTF->Branch("rpca", &rpca);
	EMTF->Branch("skip", &skip);
	EMTF->Branch("rdy", &rdy);
	EMTF->Branch("bsy", &bsy);
	EMTF->Branch("osy", &osy);
	EMTF->Branch("wof", &wof);
	EMTF->Branch("ME1a", &ME1a);
	EMTF->Branch("ME1b", &ME1b);
	EMTF->Branch("ME2", &ME2);
	EMTF->Branch("ME3", &ME3);
	EMTF->Branch("ME4", &ME4);
	EMTF->Branch("RPC", &RPC);
	EMTF->Branch("TC", &TC);
	EMTF->Branch("OC", &OC);
	EMTF->Branch("csc_me_bxn", &csc_me_bxn);
	EMTF->Branch("csc_key_wire_group", &csc_key_wire_group);
	EMTF->Branch("csc_clct_key_half_strip", &csc_clct_key_half_strip);
	EMTF->Branch("csc_quality", &csc_quality);
	EMTF->Branch("csc_clct_pattern", &csc_clct_pattern);
	EMTF->Branch("csc_id", &csc_id);
	EMTF->Branch("csc_epc", &csc_epc);
	EMTF->Branch("csc_station", &csc_station);
	EMTF->Branch("csc_tbin_num", &csc_tbin_num);
	EMTF->Branch("csc_bc0", &csc_bc0);
	EMTF->Branch("csc_bxe", &csc_bxe);
	EMTF->Branch("csc_lr", &csc_lr);
	EMTF->Branch("csc_afff", &csc_afff);
	EMTF->Branch("csc_cik", &csc_cik);
	EMTF->Branch("csc_nit", &csc_nit);
	EMTF->Branch("csc_afef", &csc_afef);
	EMTF->Branch("csc_se", &csc_se);
	EMTF->Branch("csc_sm", &csc_sm);
	EMTF->Branch("csc_af", &csc_af);
	EMTF->Branch("csc_vp", &csc_vp);
	EMTF->Branch("rpc_prt_delay", &rpc_prt_delay);
	EMTF->Branch("rpc_partition_num", &rpc_partition_num);
	EMTF->Branch("rpc_partition_data", &rpc_partition_data);
	EMTF->Branch("rpc_bcn", &rpc_bcn);
	EMTF->Branch("rpc_lb", &rpc_lb);
	EMTF->Branch("rpc_link_number", &rpc_link_number);
	EMTF->Branch("rpc_bxn", &rpc_bxn);
	EMTF->Branch("rpc_tbin", &rpc_tbin);
	EMTF->Branch("rpc_eod", &rpc_eod);
	EMTF->Branch("rpc_bc0", &rpc_bc0);
	EMTF->Branch("track_pt_lut_address", &track_pt_lut_address);
	EMTF->Branch("track_phi_full", &track_phi_full);
	EMTF->Branch("track_phi_gmt", &track_phi_gmt);
	EMTF->Branch("track_eta", &track_eta);
	EMTF->Branch("track_pt", &track_pt);
	EMTF->Branch("track_quality", &track_quality);
	EMTF->Branch("track_bx", &track_bx);
	EMTF->Branch("track_me4_id", &track_me4_id);
	EMTF->Branch("track_me3_id", &track_me3_id);
	EMTF->Branch("track_me2_id", &track_me2_id);
	EMTF->Branch("track_me1_id", &track_me1_id);
	EMTF->Branch("track_me4_tbin", &track_me4_tbin);
	EMTF->Branch("track_me3_tbin", &track_me3_tbin);
	EMTF->Branch("track_me2_tbin", &track_me2_tbin);
	EMTF->Branch("track_me1_tbin", &track_me1_tbin);
	EMTF->Branch("track_tbin_num", &track_tbin_num);
	EMTF->Branch("track_hl", &track_hl);
	EMTF->Branch("track_c", &track_c);
	EMTF->Branch("track_vc", &track_vc);
	EMTF->Branch("track_vt", &track_vt);
	EMTF->Branch("track_se", &track_se);
	EMTF->Branch("track_bc0", &track_bc0);
	EMTF->Branch("trailer_crc22", &trailer_crc22);
	EMTF->Branch("trailer_ddcrs_bid", &trailer_ddcrs_bid);
	EMTF->Branch("trailer_ddcrs_lf", &trailer_ddcrs_lf);
	EMTF->Branch("trailer_spcrs_scc", &trailer_spcrs_scc);
	EMTF->Branch("trailer_l1a", &trailer_l1a);
	EMTF->Branch("trailer_yy", &trailer_yy);
	EMTF->Branch("trailer_mm", &trailer_mm);
	EMTF->Branch("trailer_dd", &trailer_dd);
	EMTF->Branch("trailer_sp_ladr", &trailer_sp_ladr);
	EMTF->Branch("trailer_sp_ersv", &trailer_sp_ersv);
	EMTF->Branch("trailer_sp_padr", &trailer_sp_padr);
	EMTF->Branch("trailer_lfff", &trailer_lfff);
	EMTF->Branch("trailer_bb", &trailer_bb);
	EMTF->Branch("postpayload_trailer_crc32", &postpayload_trailer_crc32);
	EMTF->Branch("postpayload_trailer_bxid", &postpayload_trailer_bxid);
	EMTF->Branch("postpayload_trailer_block_number", &postpayload_trailer_block_number);
	EMTF->Branch("postpayload_trailer_lv1_id", &postpayload_trailer_lv1_id);
	EMTF->Branch("amc13_trailer_evt_lgth", &amc13_trailer_evt_lgth);
	EMTF->Branch("amc13_trailer_crc16", &amc13_trailer_crc16);
	EMTF->Branch("amc13_trailer_evt_stat", &amc13_trailer_evt_stat);
	EMTF->Branch("amc13_trailer_tts", &amc13_trailer_tts);
	EMTF->Branch("amc13_trailer_c", &amc13_trailer_c);
	EMTF->Branch("amc13_trailer_f", &amc13_trailer_f);
	EMTF->Branch("amc13_trailer_t", &amc13_trailer_t);
	EMTF->Branch("amc13_trailer_r", &amc13_trailer_r);

	//}}}
	//****************************************************************************
	//                END CREATE NTUPLE                  
	//**************************************************************************** 


}


EMTFUnpacker::~EMTFUnpacker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
	ntuple->Write();
	ntuple->Close();
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
	std::cout << "fedData.data(): " << fedData.data() << std::endl;
	//LogDebug("CSCTFUnpacker|produce");
        //if( monitor ) monitor->process((unsigned short*)fedData.data());
        //unsigned int unpacking_status = tfEvent.unpack((unsigned short*)fedData.data(),fedData.size()/2);
	
	const char* data_release = "test";	
	mtf7::emutf_operator_builder * my_operator = new mtf7::emutf_operator_builder();
	mtf7::emutf_data_operator * my_data_operator = my_operator -> get_data_operator(data_release);
	mtf7::emutf_event * _unpacked_event = new mtf7::emutf_event();
	// setting the event where the unpacked information will be stored
	my_data_operator -> set_unpacked_event_info_ptr(_unpacked_event);
	// // and now unpacking
	const mtf7::word_64bit * buffer_pointer = (const long unsigned int*)fedData.data();
	my_data_operator->unpack(buffer_pointer);
	// std::cout << "unpacked event AMC13 Header evt_ty    : " << _unpacked_event -> _amc13_header_evt_ty << std::endl;
	// std::cout << "unpacked event AMC13 Header lv1_id    : " << _unpacked_event -> _amc13_header_lv1_id << std::endl;
	// std::cout << "unpacked event AMC13 Header bx_id     : " << _unpacked_event -> _amc13_header_bx_id << std::endl;
	// std::cout << "unpacked event AMC13 Header source_id : " << _unpacked_event -> _amc13_header_source_id << std::endl;
	// std::cout << "unpacked event AMC13 Header header_x  : " << _unpacked_event -> _amc13_header_x << std::endl;
	// std::cout << "unpacked event AMC13 Header header_h  : " << _unpacked_event -> _amc13_header_h << std::endl;
	// std::cout << "unpacked event AMC13 Header header_fov: " << _unpacked_event -> _amc13_header_fov << std::endl;

	//----This works!  The AMC13 (and CDF) header is successfully unpacked!
	// This then tries to unpack the header block operator, but breaks because there is no header block operator present.
	//It would be nice if the unpacker jumped to the next section if formatting characters were not found, rather than break out of the event.

		//****************************************************************************
		//                BEGIN FILLING VARIABLES FOR NTUPLE                  
		//****************************************************************************

		//{{{
		//AMC13 HEADER 

		amc13_header_orn       = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_orn       ;                                        
		amc13_header_lv1_id    = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_lv1_id    ;                                        
		amc13_header_bx_id     = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_bx_id     ;                                        
		amc13_header_source_id = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_source_id ;                                       
		amc13_header_evt_ty    = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_evt_ty    ;                                       
		amc13_header_fov       = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_fov       ;                                         
		amc13_header_ufov      = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_ufov      ;                                         
		amc13_header_res       = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_res       ;                                         
		amc13_header_namc      = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_namc      ;                                         
		amc13_header_h         = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_h         ;                                         
		amc13_header_x         = (int)_unpacked_event ->_emutf_amc13_header_block->_amc13_header_x         ;                                         

		//PREPAYLOAD HEADER
		for(unsigned int i = 0; i < _unpacked_event->_emutf_prepayload_header_block_vector.size(); i++){
			prepayload_l        .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_l         );
			prepayload_m        .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_m         );
			prepayload_s        .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_s         );
			prepayload_e        .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_e         );
			prepayload_p        .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_p         );
			prepayload_v        .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_v         );
			prepayload_c        .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_c         );
			prepayload_amcno    .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_amcno     );
			prepayload_blkNo    .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_blkNo     );
			prepayload_amcn_size.push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_amcn_size );
			prepayload_boardid  .push_back( (int)_unpacked_event -> _emutf_prepayload_header_block_vector.at(i)->_prepayload_boardid   );
		}

		//AMC HEADER
		for(unsigned int i = 0; i < _unpacked_event->_emutf_amc_header_block_vector.size(); i++){
			amc_header_amc_number  .push_back( (int)_unpacked_event ->_emutf_amc_header_block_vector.at(i)->_amc_header_amc_number        );
			amc_header_bx_id       .push_back( (int)_unpacked_event ->_emutf_amc_header_block_vector.at(i)->_amc_header_bx_id             );
			amc_header_orbit_number.push_back( (int)_unpacked_event ->_emutf_amc_header_block_vector.at(i)->_amc_header_orbit_number      );
			amc_header_board_id    .push_back( (int)_unpacked_event ->_emutf_amc_header_block_vector.at(i)->_amc_header_board_id          );
			amc_header_lv1_id      .push_back( (int)_unpacked_event ->_emutf_amc_header_block_vector.at(i)->_amc_header_lv1_id            );
			amc_header_data_length .push_back( (int)_unpacked_event ->_emutf_amc_header_block_vector.at(i)->_amc_header_data_length       );
			amc_header_user_id     .push_back( (int)_unpacked_event ->_emutf_amc_header_block_vector.at(i)->_amc_header_user_id           );
		}	

		//EVENT RECORD HEADER
		for(unsigned int i = 0; i < _unpacked_event->_emutf_header_block_vector.size(); i++){
			l1a    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_l1a     );
			bxn    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_bxn     );
			sp_ts  .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_sp_ts   );
			sp_ersv.push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_sp_ersv );
			sp_addr.push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_sp_addr );
			tbin   .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_tbin    );
			ddm    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_ddm     );
			spa    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_spa     );
			rpca   .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_rpca    );
			skip   .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_skip    );
			rdy    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_rdy     );
			bsy    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_bsy     );
			osy    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_osy     );
			wof    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_wof     );
			ME1a   .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_ME1a    );
			ME1b   .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_ME1b    );
			ME2    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_ME2     );
			ME3    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_ME3     );
			ME4    .push_back( (int)_unpacked_event -> _emutf_header_block_vector.at(i)->_ME4     );
		}

		//BLOCK OF COUNTERS
		for(unsigned int i = 0; i < _unpacked_event->_emutf_counter_block_vector.size(); i++){
			TC.push_back( (int)_unpacked_event -> _emutf_counter_block_vector.at(i)->_TC); 
			OC.push_back( (int)_unpacked_event -> _emutf_counter_block_vector.at(i)->_OC); 
		}


		//CSC ME DATA RECORD
		for(unsigned int i = 0; i < _unpacked_event->_emutf_cscmedata_block_vector.size(); i++){
			csc_me_bxn             .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_me_bxn              );
			csc_key_wire_group     .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_key_wire_group      );
			csc_clct_key_half_strip.push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_clct_key_half_strip );
			csc_quality            .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_quality             );
			csc_clct_pattern       .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_clct_pattern        );
			csc_id                 .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_id                  );
			csc_epc                .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_epc                 );
			csc_station            .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_station             );
			csc_tbin_num           .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_tbin_num            );
			csc_bc0                .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_bc0                 );
			csc_bxe                .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_bxe                 );
			csc_lr                 .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_lr                  );
			csc_afff               .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_afff                );
			csc_cik                .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_cik                 );
			csc_nit                .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_nit                 );
			csc_afef               .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_afef                );
			csc_se                 .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_se                  );
			csc_sm                 .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_sm                  );
			csc_af                 .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_af                  );
			csc_vp                 .push_back( (int)_unpacked_event -> _emutf_cscmedata_block_vector.at(i)->_csc_vp                  );
		}


		//RPC DATA RECORD
		for(unsigned int i = 0; i < _unpacked_event->_emutf_rpcdata_block_vector.size(); i++){
			rpc_prt_delay     .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_prt_delay      );
			rpc_partition_num .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_partition_num  );
			rpc_partition_data.push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_partition_data );
			rpc_bcn           .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_bcn            );
			rpc_lb            .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_lb             );
			rpc_link_number   .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_link_number    );
			rpc_bxn           .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_bxn            );
			rpc_tbin          .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_tbin           );
			rpc_eod           .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_eod            );
			rpc_bc0           .push_back( (int)_unpacked_event -> _emutf_rpcdata_block_vector.at(i)->_rpc_bc0            );
		}


		//SP DATA RECORD
		for(unsigned int i = 0; i < _unpacked_event->_emutf_spoutputdata_block_vector.size(); i++){
			track_pt_lut_address.push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_pt_lut_address );
			track_phi_full      .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_phi_full       );
			track_phi_gmt       .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_phi_gmt        );
			track_eta           .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_eta            );
			track_pt            .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_pt             );
			track_quality       .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_quality        );
			track_bx            .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_bx             );
			track_me4_id        .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me4_id         );
			track_me3_id        .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me3_id         );
			track_me2_id        .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me2_id         );
			track_me1_id        .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me1_id         );
			track_me4_tbin      .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me4_tbin       );
			track_me3_tbin      .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me3_tbin       );
			track_me2_tbin      .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me2_tbin       );
			track_me1_tbin      .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_me1_tbin       );
			track_tbin_num      .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_tbin_num       );
			track_hl            .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_hl             );
			track_c             .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_c              );
			track_vc            .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_vc             );
			track_vt            .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_vt             );
			track_se            .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_se             );
			track_bc0           .push_back( (int)_unpacked_event -> _emutf_spoutputdata_block_vector.at(i)->_track_bc0            );
		}

		//EVENT RECORD TRAILER
		for(unsigned int i = 0; i < _unpacked_event->_emutf_trailer_block_vector.size(); i++){
			trailer_crc22    .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_crc22     );
			trailer_ddcrs_bid.push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_ddcrs_bid );
			trailer_ddcrs_lf .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_ddcrs_lf  );
			trailer_spcrs_scc.push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_spcrs_scc );
			trailer_l1a      .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_l1a       );
			trailer_yy       .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_yy        );
			trailer_mm       .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_mm        );
			trailer_dd       .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_dd        );
			trailer_sp_ladr  .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_sp_ladr   );
			trailer_sp_ersv  .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_sp_ersv   );
			trailer_sp_padr  .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_sp_padr   );
			trailer_lfff     .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_lfff      );
			trailer_bb       .push_back( (int)_unpacked_event -> _emutf_trailer_block_vector.at(i)->_trailer_bb        );
		}

		//AMC TRAILER
		for(unsigned int i = 0; i < _unpacked_event->_emutf_amc_trailer_block_vector.size(); i++){
			amc_trailer_lv1_id     .push_back( (int)_unpacked_event -> _emutf_amc_trailer_block_vector.at(i)->_amc_trailer_lv1_id      );               
			amc_trailer_crc32      .push_back( (int)_unpacked_event -> _emutf_amc_trailer_block_vector.at(i)->_amc_trailer_crc32       );               
			amc_trailer_data_length.push_back( (int)_unpacked_event -> _emutf_amc_trailer_block_vector.at(i)->_amc_trailer_data_length ); 
		}

		//POST PAYLOAD TRAILER
		postpayload_trailer_crc32       = (int)_unpacked_event ->_emutf_postpayload_trailer_block->_postpayload_trailer_crc32        ;                                       
		postpayload_trailer_bxid        = (int)_unpacked_event ->_emutf_postpayload_trailer_block->_postpayload_trailer_bxid         ;                                        
		postpayload_trailer_block_number= (int)_unpacked_event ->_emutf_postpayload_trailer_block->_postpayload_trailer_block_number ;                                         
		postpayload_trailer_lv1_id      = (int)_unpacked_event ->_emutf_postpayload_trailer_block->_postpayload_trailer_lv1_id       ;                                          


		//AMC13 TRAILER
		amc13_trailer_evt_lgth= (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_evt_lgth ;                                             
		amc13_trailer_crc16   = (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_crc16    ;                                             
		amc13_trailer_evt_stat= (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_evt_stat ;                                             
		amc13_trailer_tts     = (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_tts      ;                                             
		amc13_trailer_c       = (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_c        ;                                             
		amc13_trailer_f       = (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_f        ;                                             
		amc13_trailer_t       = (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_t        ;                                             
		amc13_trailer_r       = (int)_unpacked_event -> _emutf_amc13_trailer_block->_amc13_trailer_r        ;                                             


		//EMTF->Fill(); //This worked, but I think that the tree should be filled at the end		
		//}}}
		//****************************************************************************
		//                END FILLING VARIABLES FOR NTUPLE                  
		//****************************************************************************


	}
        EMTF->Fill();	
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

