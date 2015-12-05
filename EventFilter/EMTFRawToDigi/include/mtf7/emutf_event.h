#ifndef __MTF7_EMU_EVENT__ 
#define __MTF7_EMU_EVENT__ 

#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include <iostream>
#include <vector>

namespace mtf7{

  // AMC13 event header
  struct emutf_amc13_header_block {

    word_32bit _amc13_header_orn, _amc13_header_lv1_id;
    word_16bit _amc13_header_bx_id, _amc13_header_source_id;
    word_8bit _amc13_header_evt_ty, _amc13_header_fov, _amc13_header_ufov, _amc13_header_res, _amc13_header_namc;
    word_1bit _amc13_header_h, _amc13_header_x;

    emutf_amc13_header_block(){ std::cout << "@HEAR ME: Constructing header" << std::endl; clear_block(); }

    void clear_block(){
        _amc13_header_orn = 0;
        _amc13_header_lv1_id = 0;
        _amc13_header_bx_id = 0;
        _amc13_header_source_id = 0;
        _amc13_header_evt_ty = 0;
        _amc13_header_fov = 0;
        _amc13_header_ufov = 0;
        _amc13_header_res = 0;
        _amc13_header_namc = 0;
        _amc13_header_h = 0;
        _amc13_header_x = 0;
    }

  };


    // preheader
    struct emutf_prepayload_header_block{

        word_1bit _prepayload_l, _prepayload_m, _prepayload_s, _prepayload_e, _prepayload_p, _prepayload_v, _prepayload_c;
        word_8bit _prepayload_amcno;
        word_16bit _prepayload_blkNo;
        word_32bit _prepayload_amcn_size, _prepayload_boardid;

        void clear_block(){
            _prepayload_l = 0; 
            _prepayload_m = 0;
            _prepayload_s = 0; 
            _prepayload_e = 0; 
            _prepayload_p = 0;
            _prepayload_v = 0;
            _prepayload_c = 0;
            _prepayload_amcno = 0;
            _prepayload_blkNo = 0;
            _prepayload_amcn_size = 0;
            _prepayload_boardid = 0;
        }
    };

    // amc header
    struct emutf_amc_header_block{

        word_8bit _amc_header_amc_number;
        word_16bit _amc_header_bx_id, _amc_header_orbit_number, _amc_header_board_id;
        word_32bit _amc_header_lv1_id, _amc_header_data_length, _amc_header_user_id;

        void clear_block(){
        _amc_header_amc_number = 0;
	_amc_header_bx_id= 0;
	_amc_header_orbit_number= 0;
	_amc_header_board_id= 0;
	_amc_header_lv1_id= 0;
	_amc_header_data_length= 0;
	_amc_header_user_id= 0;
	}
    };

    // amc trailer
    struct emutf_amc_trailer_block{

        word_16bit _amc_trailer_lv1_id;
        word_32bit _amc_trailer_crc32, _amc_trailer_data_length;

        void clear_block(){
        _amc_trailer_lv1_id = 0;
	_amc_trailer_crc32= 0;
	_amc_trailer_data_length= 0;
	}
    };


  // Event Record Header
  struct emutf_header_block {

    word_32bit _l1a, _RPC;    
    word_16bit _bxn, _ME1a, _ME1b, _ME2, _ME3, _ME4;
    word_8bit  _sp_ts, _sp_ersv, _sp_addr, _tbin;   
    word_1bit  _ddm, _spa, _rpca, _skip, _rdy, _bsy, _osy, _wof;    

    void clear_block(){
    _l1a = 0;
    _bxn = 0;
    
    _sp_ts   = 0; 
    _sp_ersv = 0; 
    _sp_addr = 0; 
    _tbin    = 0;
    
    _ddm  = false; 
    _spa  = false;
    _rpca = false;
    _skip = false; 
    _rdy  = false; 
    _bsy  = false; 
    _osy  = false; 
    _wof  = false;    
    
    _ME1a = 0;
    _ME1b = 0; 
    _ME2  = 0; 
    _ME3  = 0; 
    _ME4  = 0;

    _RPC = 0;

    }
  };

  // Block of Counters
  struct emutf_counter_block{
 
    word_32bit _TC, _OC;

    void clear_block(){
    _TC  = 0;
    _OC  = 0;
    }
  };

  // CSC ME Data Record
  struct emutf_cscmedata_block{
  
    word_16bit _csc_me_bxn;
    word_8bit _csc_key_wire_group, _csc_clct_key_half_strip, _csc_quality, _csc_clct_pattern, _csc_id, _csc_epc, _csc_station, _csc_tbin_num;
    word_1bit _csc_bc0, _csc_bxe, _csc_lr, _csc_afff, _csc_cik, _csc_nit, _csc_afef, _csc_se, _csc_sm, _csc_af, _csc_vp;
  
    void clear_block(){
    _csc_me_bxn = 0;
    _csc_key_wire_group = 0;
    _csc_clct_key_half_strip = 0;
    _csc_quality = 0;
    _csc_clct_pattern = 0;
    _csc_id = 0;
    _csc_epc = 0;
    _csc_station = 0;
    _csc_tbin_num = 0;

    _csc_bc0 = false;
    _csc_bxe = false;
    _csc_lr = false;
    _csc_afff = false;
    _csc_cik = false;
    _csc_nit = false;
    _csc_afef = false;
    _csc_se = false;
    _csc_sm = false;
    _csc_af = false;
    _csc_vp = false;
    }
  };

  // RPC Data Record
  struct emutf_rpcdata_block{
  
    word_8bit _rpc_prt_delay, _rpc_partition_num, _rpc_partition_data, _rpc_bcn, _rpc_lb, _rpc_link_number, _rpc_bxn, _rpc_tbin;
    word_1bit _rpc_eod, _rpc_bc0;

    void clear_block(){
    _rpc_prt_delay = 0;
    _rpc_partition_num = 0;
    _rpc_partition_data = 0;
    _rpc_bcn = 0;
    _rpc_lb = 0;
    _rpc_link_number = 0;
    _rpc_bxn = 0;
    _rpc_tbin = 0;

    _rpc_eod = false;
    _rpc_bc0 = false;
    }

  };

  // SP Output Data Record
  struct emutf_spoutputdata_block{
    word_32bit _track_pt_lut_address;
    word_16bit _track_phi_inner, _track_phi_outer, _track_eta, _track_pt;
    word_8bit _track_quality, _track_bx, _track_me4_id, _track_me3_id, _track_me2_id, _track_me1_id, _track_tbin_num, _track_me4_tbin, _track_me3_tbin, _track_me2_tbin, _track_me1_tbin;
    word_1bit _track_hl, _track_c, _track_vc, _track_se, _track_bc0; 

    void clear_block(){
    _track_pt_lut_address = 0;
    _track_phi_inner = 0;
    _track_phi_outer = 0;
    _track_eta = 0;
    _track_pt = 0;
    _track_quality = 0;
    _track_bx = 0;
    _track_me4_id = 0;
    _track_me3_id = 0;
    _track_me2_id = 0;
    _track_me1_id = 0;
    _track_me4_tbin = 0;
    _track_me3_tbin = 0;
    _track_me2_tbin = 0;
    _track_me1_tbin = 0;
    _track_tbin_num = 0;

    _track_hl = false;
    _track_c = false;
    _track_vc = false;
    _track_se = false;
    _track_bc0 = false;
    }

  };

  // Event Record Trailer
  struct emutf_trailer_block{
    word_32bit _trailer_crc22;
    word_16bit _trailer_ddcrs_bid, _trailer_spcrs_scc;
    word_8bit _trailer_l1a, _trailer_yy, _trailer_mm, _trailer_dd, _trailer_sp_ladr, _trailer_sp_ersv, _trailer_sp_padr, _trailer_ddcrs_lf;
    word_1bit _trailer_lfff, _trailer_bb, _trailer_lp, _trailer_hp;
    
    void clear_block(){
    _trailer_crc22 = 0;
    _trailer_ddcrs_bid = 0;
    _trailer_ddcrs_lf = 0;
    _trailer_spcrs_scc = 0;
    _trailer_l1a = 0;
    _trailer_yy = 0;
    _trailer_mm = 0;
    _trailer_dd = 0;
    _trailer_sp_ladr = 0;
    _trailer_sp_ersv = 0;
    _trailer_sp_padr = 0;

    _trailer_lfff = false;
    _trailer_bb = false;
    }
  };


  //postpayload trailer
  struct emutf_postpayload_trailer_block{
  
    word_32bit _postpayload_trailer_crc32;
    word_16bit _postpayload_trailer_bxid;
    word_8bit _postpayload_trailer_block_number, _postpayload_trailer_lv1_id;

    void clear_block(){
    _postpayload_trailer_crc32 = 0;
    _postpayload_trailer_bxid = 0;
    _postpayload_trailer_block_number = 0;
    _postpayload_trailer_lv1_id = 0;
    }
  };

  // AMC13 Trailer
  struct emutf_amc13_trailer_block{
  
    word_32bit _amc13_trailer_evt_lgth;
    word_16bit _amc13_trailer_crc16;
    word_8bit _amc13_trailer_evt_stat, _amc13_trailer_tts;
    word_1bit _amc13_trailer_c, _amc13_trailer_f, _amc13_trailer_t, _amc13_trailer_r;

    void clear_block(){
    _amc13_trailer_evt_lgth = 0;
    _amc13_trailer_crc16 = 0;
    _amc13_trailer_evt_stat = 0;
    _amc13_trailer_tts = 0;
    _amc13_trailer_c = 0;
    _amc13_trailer_f = 0;
    _amc13_trailer_t = 0;
    _amc13_trailer_r = 0;
    }
  };




  // EMUTF Full Event
  struct emutf_event {

    // In each event there are one AMC13 header and trailer and nAMC data blocks. nAMC is a value written in the AMC13 header

    // AMC13 event header
    emutf_amc13_header_block *_emutf_amc13_header_block = new emutf_amc13_header_block();
    // Vector of event record preheader
    std::vector<emutf_prepayload_header_block *> _emutf_prepayload_header_block_vector;
    // Vector of amc header
    std::vector<emutf_amc_header_block *> _emutf_amc_header_block_vector;
    // Vector of event record header
    std::vector<emutf_header_block *> _emutf_header_block_vector;
    // Vector of block of counters
    std::vector<emutf_counter_block *> _emutf_counter_block_vector;
    // Vector of CSC ME data record
    std::vector<emutf_cscmedata_block *> _emutf_cscmedata_block_vector;
    // Vector of RPC data record
    std::vector<emutf_rpcdata_block *> _emutf_rpcdata_block_vector;
    // Vector of SP output data record
    std::vector<emutf_spoutputdata_block *> _emutf_spoutputdata_block_vector;
     // Vector of amc trailer
    std::vector<emutf_amc_trailer_block *> _emutf_amc_trailer_block_vector;
    // Vector of event record trailer
    std::vector<emutf_trailer_block *> _emutf_trailer_block_vector;
    // postpayload trailer 
    emutf_postpayload_trailer_block *_emutf_postpayload_trailer_block = new emutf_postpayload_trailer_block();
    // AMC13 event trailer 
    emutf_amc13_trailer_block *_emutf_amc13_trailer_block = new emutf_amc13_trailer_block();

  };


  // Clear full event
  static void clear_emutf_event( emutf_event *event_info ){

    event_info -> _emutf_amc13_header_block -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_prepayload_header_block_vector.size(); i++)
        event_info -> _emutf_prepayload_header_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_amc_header_block_vector.size(); i++)
        event_info -> _emutf_amc_header_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_header_block_vector.size(); i++)
        event_info -> _emutf_header_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_counter_block_vector.size(); i++)
        event_info -> _emutf_counter_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_cscmedata_block_vector.size(); i++)
        event_info -> _emutf_cscmedata_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_rpcdata_block_vector.size(); i++)
        event_info -> _emutf_rpcdata_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_spoutputdata_block_vector.size(); i++)
        event_info -> _emutf_spoutputdata_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_amc_trailer_block_vector.size(); i++)
        event_info -> _emutf_amc_trailer_block_vector.at(i) -> clear_block();
    for (unsigned int i=0; i<= event_info -> _emutf_trailer_block_vector.size(); i++)
        event_info -> _emutf_trailer_block_vector.at(i) -> clear_block();
    event_info -> _emutf_postpayload_trailer_block -> clear_block();
    event_info -> _emutf_amc13_trailer_block -> clear_block();

  }



}
#endif
