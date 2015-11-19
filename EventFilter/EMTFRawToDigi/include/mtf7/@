#ifndef __MTF7_EMU_EVENT__ 
#define __MTF7_EMU_EVENT__ 

#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"

namespace mtf7{

  struct emutf_event {

    // AMC13 event header
    word_32bit _amc13_header_orn, _amc13_header_lv1_id;
    word_16bit _amc13_header_bx_id, _amc13_header_source_id;
    word_8bit _amc13_header_evt_ty, _amc13_header_fov, _amc13_header_ufov, _amc13_header_res, _amc13_header_namc;
    word_1bit _amc13_header_h, _amc13_header_x;

    // Event Record Header
    word_32bit _l1a;    
    word_16bit _bxn, _ME1a, _ME1b, _ME2, _ME3, _ME4;
    word_8bit  _sp_ts, _sp_ersv, _sp_addr, _tbin;   
    word_1bit  _ddm, _spa, _rpca, _skip, _rdy, _bsy, _osy, _wof;    

    // Block of Counters
    word_32bit _RPC, _TC, _OC;

    // CSC ME Data Record
    word_16bit _csc_me_bxn;
    word_8bit _csc_key_wire_group, _csc_clct_key_half_strip, _csc_quality, _csc_clct_pattern, _csc_id, _csc_epc, _csc_station, _csc_tbin_num;
    word_1bit _csc_bc0, _csc_bxe, _csc_lr, _csc_afff, _csc_cik, _csc_nit, _csc_afef, _csc_se, _csc_sm, _csc_af, _csc_vp;

    // RPC Data Record
    word_8bit _rpc_prt_delay, _rpc_partition_num, _rpc_partition_data, _rpc_bcn, _rpc_lb, _rpc_link_number, _rpc_bxn, _rpc_tbin;
    word_1bit _rpc_eod, _rpc_bc0;

    // SP Output Data Record
    word_32bit _track_pt_lut_address;
    word_16bit _track_phi_inner, _track_phi_outer, _track_eta, _track_pt;
    word_8bit _track_quality, _track_bx, _track_me4_id, _track_me3_id, _track_me2_id, _track_me1_id, _track_tbin_num, _track_me4_tbin, _track_me3_tbin, _track_me2_tbin, _track_me1_tbin;
    word_1bit _track_hl, _track_c, _track_vc, _track_se, _track_bc0; 

    // Event Record Trailer
    word_32bit _trailer_crc22;
    word_16bit _trailer_ddcrs_bid, _trailer_spcrs_scc;
    word_8bit _trailer_l1a, _trailer_yy, _trailer_mm, _trailer_dd, _trailer_sp_ladr, _trailer_sp_ersv, _trailer_sp_padr, _trailer_ddcrs_lf;
    word_1bit _trailer_lfff, _trailer_bb, _trailer_lp, _trailer_hp;

    // AMC13 Trailer
    word_32bit _amc13_trailer_evt_lgth;
    word_16bit _amc13_trailer_crc16;
    word_8bit _amc13_trailer_evt_stat, _amc13_trailer_tts;
    word_1bit _amc13_trailer_c, _amc13_trailer_f, _amc13_trailer_t, _amc13_trailer_r;

  };

  static void clear_emutf_event( emutf_event *event_info ){

    // AMC13 event header
    event_info -> _amc13_header_orn = 0;
    event_info -> _amc13_header_lv1_id = 0;
    event_info -> _amc13_header_bx_id = 0;
    event_info -> _amc13_header_source_id = 0;
    event_info -> _amc13_header_evt_ty = 0;
    event_info -> _amc13_header_fov = 0;
    event_info -> _amc13_header_ufov = 0;
    event_info -> _amc13_header_res = 0;
    event_info -> _amc13_header_namc = 0;
    event_info -> _amc13_header_h = 0;
    event_info -> _amc13_header_x = 0;

    // Event header
    event_info -> _l1a = 0;
    event_info -> _bxn = 0;
    
    event_info -> _sp_ts   = 0; 
    event_info -> _sp_ersv = 0; 
    event_info -> _sp_addr = 0; 
    event_info -> _tbin    = 0;
    
    event_info -> _ddm  = false; 
    event_info -> _spa  = false;
    event_info -> _rpca = false;
    event_info -> _skip = false; 
    event_info -> _rdy  = false; 
    event_info -> _bsy  = false; 
    event_info -> _osy  = false; 
    event_info -> _wof  = false;    
    
    event_info -> _ME1a = 0;
    event_info -> _ME1b = 0; 
    event_info -> _ME2  = 0; 
    event_info -> _ME3  = 0; 
    event_info -> _ME4  = 0;
    
    event_info -> _RPC = 0; 
    event_info -> _TC  = 0;
    event_info -> _OC  = 0;
    
    // CSC ME Data Record
    event_info -> _csc_me_bxn = 0;
    event_info -> _csc_key_wire_group = 0;
    event_info -> _csc_clct_key_half_strip = 0;
    event_info -> _csc_quality = 0;
    event_info -> _csc_clct_pattern = 0;
    event_info -> _csc_id = 0;
    event_info -> _csc_epc = 0;
    event_info -> _csc_station = 0;
    event_info -> _csc_tbin_num = 0;

    event_info -> _csc_bc0 = false;
    event_info -> _csc_bxe = false;
    event_info -> _csc_lr = false;
    event_info -> _csc_afff = false;
    event_info -> _csc_cik = false;
    event_info -> _csc_nit = false;
    event_info -> _csc_afef = false;
    event_info -> _csc_se = false;
    event_info -> _csc_sm = false;
    event_info -> _csc_af = false;
    event_info -> _csc_vp = false;

    // RPC Data Record
    event_info -> _rpc_prt_delay = 0;
    event_info -> _rpc_partition_num = 0;
    event_info -> _rpc_partition_data = 0;
    event_info -> _rpc_bcn = 0;
    event_info -> _rpc_lb = 0;
    event_info -> _rpc_link_number = 0;
    event_info -> _rpc_bxn = 0;
    event_info -> _rpc_tbin = 0;

    event_info -> _rpc_eod = false;
    event_info -> _rpc_bc0 = false;

    // SP Output Data Record
    event_info -> _track_pt_lut_address = 0;
    event_info -> _track_phi_inner = 0;
    event_info -> _track_phi_outer = 0;
    event_info -> _track_eta = 0;
    event_info -> _track_pt = 0;
    event_info -> _track_quality = 0;
    event_info -> _track_bx = 0;
    event_info -> _track_me4_id = 0;
    event_info -> _track_me3_id = 0;
    event_info -> _track_me2_id = 0;
    event_info -> _track_me1_id = 0;
    event_info -> _track_me4_tbin = 0;
    event_info -> _track_me3_tbin = 0;
    event_info -> _track_me2_tbin = 0;
    event_info -> _track_me1_tbin = 0;
    event_info -> _track_tbin_num = 0;

    event_info -> _track_hl = false;
    event_info -> _track_c = false;
    event_info -> _track_vc = false;
    event_info -> _track_se = false;
    event_info -> _track_bc0 = false;

    // Event Record Trailer
    event_info -> _trailer_crc22 = 0;
    event_info -> _trailer_ddcrs_bid = 0;
    event_info -> _trailer_ddcrs_lf = 0;
    event_info -> _trailer_spcrs_scc = 0;
    event_info -> _trailer_l1a = 0;
    event_info -> _trailer_yy = 0;
    event_info -> _trailer_mm = 0;
    event_info -> _trailer_dd = 0;
    event_info -> _trailer_sp_ladr = 0;
    event_info -> _trailer_sp_ersv = 0;
    event_info -> _trailer_sp_padr = 0;

    event_info -> _trailer_lfff = false;
    event_info -> _trailer_bb = false;

    // AMC13 event trailer
    event_info -> _amc13_trailer_evt_lgth = 0;
    event_info -> _amc13_trailer_crc16 = 0;
    event_info -> _amc13_trailer_evt_stat = 0;
    event_info -> _amc13_trailer_tts = 0;
    event_info -> _amc13_trailer_c = 0;
    event_info -> _amc13_trailer_f = 0;
    event_info -> _amc13_trailer_t = 0;
    event_info -> _amc13_trailer_r = 0;

  }

}

#endif
