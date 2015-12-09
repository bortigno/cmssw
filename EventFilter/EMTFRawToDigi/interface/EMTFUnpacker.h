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

		TFile *ntuple;
		TTree *EMTF;
		//****************************************************************************
		//                BEGIN VARIABLE INITIALIZATION FOR NTUPLE                  
		//****************************************************************************

		//{{{
		int amc13_header_orn;
		int amc13_header_lv1_id;
		int amc13_header_bx_id;
		int amc13_header_source_id;
		int amc13_header_evt_ty;
		int amc13_header_fov;
		int amc13_header_ufov;
		int amc13_header_res;
		int amc13_header_namc;
		int amc13_header_h;
		int amc13_header_x;
		std::vector<int> prepayload_l;
		std::vector<int> prepayload_m;
		std::vector<int> prepayload_s;
		std::vector<int> prepayload_e;
		std::vector<int> prepayload_p;
		std::vector<int> prepayload_v;
		std::vector<int> prepayload_c;
		std::vector<int> prepayload_amcno;
		std::vector<int> prepayload_blkNo;
		std::vector<int> prepayload_amcn_size;
		std::vector<int> prepayload_boardid;
		std::vector<int> amc_header_amc_number;
		std::vector<int> amc_header_bx_id;
		std::vector<int> amc_header_orbit_number;
		std::vector<int> amc_header_board_id;
		std::vector<int> amc_header_lv1_id;
		std::vector<int> amc_header_data_length;
		std::vector<int> amc_header_user_id;
		std::vector<int> amc_trailer_lv1_id;
		std::vector<int> amc_trailer_crc32;
		std::vector<int> amc_trailer_data_length;
		std::vector<int> l1a;
		std::vector<int> bxn;
		std::vector<int> sp_ts;
		std::vector<int> sp_ersv;
		std::vector<int> sp_addr;
		std::vector<int> tbin;
		std::vector<int> ddm;
		std::vector<int> spa;
		std::vector<int> rpca;
		std::vector<int> skip;
		std::vector<int> rdy;
		std::vector<int> bsy;
		std::vector<int> osy;
		std::vector<int> wof;
		std::vector<int> ME1a;
		std::vector<int> ME1b;
		std::vector<int> ME2;
		std::vector<int> ME3;
		std::vector<int> ME4;
		std::vector<int> RPC;
		std::vector<int> TC;
		std::vector<int> OC;
		std::vector<int> csc_me_bxn;
		std::vector<int> csc_key_wire_group;
		std::vector<int> csc_clct_key_half_strip;
		std::vector<int> csc_quality;
		std::vector<int> csc_clct_pattern;
		std::vector<int> csc_id;
		std::vector<int> csc_epc;
		std::vector<int> csc_station;
		std::vector<int> csc_tbin_num;
		std::vector<int> csc_bc0;
		std::vector<int> csc_bxe;
		std::vector<int> csc_lr;
		std::vector<int> csc_afff;
		std::vector<int> csc_cik;
		std::vector<int> csc_nit;
		std::vector<int> csc_afef;
		std::vector<int> csc_se;
		std::vector<int> csc_sm;
		std::vector<int> csc_af;
		std::vector<int> csc_vp;
		std::vector<int> rpc_prt_delay;
		std::vector<int> rpc_partition_num;
		std::vector<int> rpc_partition_data;
		std::vector<int> rpc_bcn;
		std::vector<int> rpc_lb;
		std::vector<int> rpc_link_number;
		std::vector<int> rpc_bxn;
		std::vector<int> rpc_tbin;
		std::vector<int> rpc_eod;
		std::vector<int> rpc_bc0;
		std::vector<int> track_pt_lut_address;
		std::vector<int> track_phi_inner;
		std::vector<int> track_phi_outer;
		std::vector<int> track_eta;
		std::vector<int> track_pt;
		std::vector<int> track_quality;
		std::vector<int> track_bx;
		std::vector<int> track_me4_id;
		std::vector<int> track_me3_id;
		std::vector<int> track_me2_id;
		std::vector<int> track_me1_id;
		std::vector<int> track_me4_tbin;
		std::vector<int> track_me3_tbin;
		std::vector<int> track_me2_tbin;
		std::vector<int> track_me1_tbin;
		std::vector<int> track_tbin_num;
		std::vector<int> track_hl;
		std::vector<int> track_c;
		std::vector<int> track_vc;
		std::vector<int> track_se;
		std::vector<int> track_bc0;
		std::vector<int> trailer_crc22;
		std::vector<int> trailer_ddcrs_bid;
		std::vector<int> trailer_ddcrs_lf;
		std::vector<int> trailer_spcrs_scc;
		std::vector<int> trailer_l1a;
		std::vector<int> trailer_yy;
		std::vector<int> trailer_mm;
		std::vector<int> trailer_dd;
		std::vector<int> trailer_sp_ladr;
		std::vector<int> trailer_sp_ersv;
		std::vector<int> trailer_sp_padr;
		std::vector<int> trailer_lfff;
		std::vector<int> trailer_bb;
		int postpayload_trailer_crc32;
		int postpayload_trailer_bxid;
		int postpayload_trailer_block_number;
		int postpayload_trailer_lv1_id;
		int amc13_trailer_evt_lgth;
		int amc13_trailer_crc16;
		int amc13_trailer_evt_stat;
		int amc13_trailer_tts;
		int amc13_trailer_c;
		int amc13_trailer_f;
		int amc13_trailer_t;
		int amc13_trailer_r;


		//}}}

		//****************************************************************************
		//                END VARIABLE INITIALIZATION FOR NTUPLE                  
		//****************************************************************************



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
