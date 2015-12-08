#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event_generator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/unity_test.h"
#include <string>
#include <iostream>
int mtf7::unity_test::run(const mtf7::emutf_event* eventA, const mtf7::emutf_event* eventB, int errorCounter){
	int temp = 0;

	if(eventA->_amc13_header_orn != eventB-> _amc13_header_orn ){
		errorCounter++;
	}
	if(eventA->_amc13_header_lv1_id != eventB-> _amc13_header_lv1_id ) errorCounter++;
	if(eventA->_amc13_header_bx_id != eventB-> _amc13_header_bx_id ) errorCounter++;
	if(eventA->_amc13_header_source_id != eventB-> _amc13_header_source_id ) errorCounter++;
	if(eventA->_amc13_header_evt_ty != eventB-> _amc13_header_evt_ty ) errorCounter++;
	if(eventA->_amc13_header_fov != eventB-> _amc13_header_fov ) errorCounter++;
	if(eventA->_amc13_header_ufov != eventB-> _amc13_header_ufov ){

		errorCounter++;
	}
	if(eventA->_amc13_header_res != eventB-> _amc13_header_res ){
		errorCounter++;
	}
	if(eventA->_amc13_header_namc != eventB-> _amc13_header_namc ){
		errorCounter++;
	}

	if(eventA->_amc13_header_h != eventB-> _amc13_header_h ) errorCounter++;
	if(eventA->_amc13_header_x != eventB-> _amc13_header_x ) errorCounter++;
	if(eventA->_l1a != eventB-> _l1a ) errorCounter++;
	if(eventA->_bxn != eventB-> _bxn ) errorCounter++;
	if(eventA->_ME1a != eventB-> _ME1a ) errorCounter++;
	if(eventA->_ME1b != eventB-> _ME1b ) errorCounter++;
	if(eventA->_ME2 != eventB-> _ME2 ) errorCounter++;
	if(eventA->_ME3 != eventB-> _ME3 ) errorCounter++;
	if(eventA->_ME4 != eventB-> _ME4 ) errorCounter++;
	if(eventA->_sp_ts != eventB-> _sp_ts ) errorCounter++;
	if(eventA->_sp_ersv != eventB-> _sp_ersv ) errorCounter++;
	if(eventA->_sp_addr != eventB-> _sp_addr ) errorCounter++;
	if(eventA->_tbin != eventB-> _tbin ) errorCounter++;
	if(eventA->_ddm != eventB-> _ddm ) errorCounter++;
	if(eventA->_spa != eventB-> _spa ) errorCounter++;
	if(eventA->_rpca != eventB-> _rpca ) errorCounter++;
	if(eventA->_skip != eventB-> _skip ) errorCounter++;
	if(eventA->_rdy != eventB-> _rdy ) errorCounter++;
	if(eventA->_bsy != eventB-> _bsy ) errorCounter++;
	if(eventA->_osy != eventB-> _osy ) errorCounter++;
	if(eventA->_wof != eventB-> _wof ) errorCounter++;
	if(eventA->_RPC != eventB-> _RPC ) errorCounter++;
	if(eventA->_TC != eventB-> _TC ) errorCounter++;
	if(eventA->_OC != eventB-> _OC ) errorCounter++;
	if(eventA->_csc_me_bxn != eventB-> _csc_me_bxn ){
		errorCounter++;

	}
	if(eventA->_csc_key_wire_group != eventB-> _csc_key_wire_group ) errorCounter++;
	if(eventA->_csc_clct_key_half_strip != eventB-> _csc_clct_key_half_strip ) errorCounter++;
	if(eventA->_csc_quality != eventB-> _csc_quality ) errorCounter++;
	if(eventA->_csc_clct_pattern != eventB-> _csc_clct_pattern ) errorCounter++;
	if(eventA->_csc_id != eventB-> _csc_id ) errorCounter++;
	if(eventA->_csc_epc != eventB-> _csc_epc ) errorCounter++;
	if(eventA->_csc_station != eventB-> _csc_station ) errorCounter++;
	if(eventA->_csc_tbin_num != eventB-> _csc_tbin_num ) errorCounter++;
	if(eventA->_csc_bc0 != eventB-> _csc_bc0 ) errorCounter++;
	if(eventA->_csc_bxe != eventB-> _csc_bxe ) errorCounter++;
	if(eventA->_csc_lr != eventB-> _csc_lr ) errorCounter++;
	if(eventA->_csc_afff != eventB-> _csc_afff ) errorCounter++;
	if(eventA->_csc_cik != eventB-> _csc_cik ) errorCounter++;
	if(eventA->_csc_nit != eventB-> _csc_nit ) errorCounter++;
	if(eventA->_csc_afef != eventB-> _csc_afef ) errorCounter++;
	if(eventA->_csc_se != eventB-> _csc_se ) errorCounter++;
	if(eventA->_csc_sm != eventB-> _csc_sm ) errorCounter++;
	if(eventA->_csc_af != eventB-> _csc_af ) errorCounter++;
	if(eventA->_csc_vp != eventB-> _csc_vp ) errorCounter++;
	if(eventA->_rpc_prt_delay != eventB-> _rpc_prt_delay ) errorCounter++;
	if(eventA->_rpc_partition_num != eventB-> _rpc_partition_num ) errorCounter++;
	if(eventA->_rpc_partition_data != eventB-> _rpc_partition_data ) errorCounter++;
	if(eventA->_rpc_bcn != eventB-> _rpc_bcn ) errorCounter++;
	if(eventA->_rpc_lb != eventB-> _rpc_lb ) errorCounter++;
	if(eventA->_rpc_link_number != eventB-> _rpc_link_number ) errorCounter++;
	if(eventA->_rpc_bxn != eventB-> _rpc_bxn ) errorCounter++;
	if(eventA->_rpc_tbin != eventB-> _rpc_tbin ) errorCounter++;
	if(eventA->_rpc_eod != eventB-> _rpc_eod ) errorCounter++;
	if(eventA->_rpc_bc0 != eventB-> _rpc_bc0 ) errorCounter++;
	if(eventA->_track_pt_lut_address != eventB-> _track_pt_lut_address ) errorCounter++;
	if(eventA->_track_phi_inner != eventB-> _track_phi_inner ){
		errorCounter++;
	}
	if(eventA->_track_phi_outer != eventB-> _track_phi_outer ) errorCounter++;
	if(eventA->_track_eta != eventB->_track_eta ){
		errorCounter++;
	}
	if(eventA->_track_pt != eventB-> _track_pt ) errorCounter++;
	if(eventA->_track_quality != eventB-> _track_quality ) errorCounter++;
	if(eventA->_track_bx != eventB-> _track_bx ) errorCounter++;
	if(eventA->_track_me4_id != eventB-> _track_me4_id ) errorCounter++;
	if(eventA->_track_me3_id != eventB-> _track_me3_id ) errorCounter++;
	if(eventA->_track_me2_id != eventB-> _track_me2_id ) errorCounter++;
	if(eventA->_track_me1_id != eventB-> _track_me1_id ) errorCounter++;
	if(eventA->_track_tbin_num != eventB-> _track_tbin_num ){
		errorCounter++;
	}
	if(eventA->_track_me4_tbin != eventB-> _track_me4_tbin ){
		errorCounter++;

	}
	if(eventA->_track_me3_tbin != eventB-> _track_me3_tbin ){
		errorCounter++;
	}
	if(eventA->_track_me2_tbin != eventB-> _track_me2_tbin ){
		errorCounter++;
	}
	if(eventA->_track_me1_tbin != eventB-> _track_me1_tbin ){
		errorCounter++;
	}
	if(eventA->_track_hl != eventB-> _track_hl ) errorCounter++;
	if(eventA->_track_c != eventB-> _track_c ) errorCounter++;
	if(eventA->_track_vc != eventB-> _track_vc ) errorCounter++;
	if(eventA->_track_se != eventB-> _track_se ) errorCounter++;
	if(eventA->_track_bc0 != eventB-> _track_bc0 ) errorCounter++;
	if(eventA->_trailer_crc22 != eventB-> _trailer_crc22 ){
		errorCounter++;
	}
	if(eventA->_trailer_ddcrs_bid != eventB-> _trailer_ddcrs_bid ){
		errorCounter++;
	}
	if(eventA->_trailer_spcrs_scc != eventB-> _trailer_spcrs_scc ) errorCounter++;
	if(eventA->_trailer_l1a != eventB-> _trailer_l1a ) errorCounter++;
	if(eventA->_trailer_yy != eventB-> _trailer_yy ) errorCounter++;
	if(eventA->_trailer_mm != eventB-> _trailer_mm ) errorCounter++;
	if(eventA->_trailer_dd != eventB-> _trailer_dd ) errorCounter++;
	if(eventA->_trailer_sp_ladr != eventB-> _trailer_sp_ladr ){
		errorCounter++;
	}
	if(eventA->_trailer_sp_ersv != eventB-> _trailer_sp_ersv ){
		errorCounter++;
	}
	if(eventA->_trailer_sp_padr != eventB-> _trailer_sp_padr ) errorCounter++;
	if(eventA->_trailer_ddcrs_lf != eventB-> _trailer_ddcrs_lf ) errorCounter++;
	if(eventA->_trailer_lfff != eventB-> _trailer_lfff ) errorCounter++;
	if(eventA->_trailer_bb != eventB-> _trailer_bb ) errorCounter++;
	if(eventA->_trailer_lp != eventB-> _trailer_lp ){
		errorCounter++;
	}
	if(eventA->_trailer_hp != eventB-> _trailer_hp ) errorCounter++;
	if(eventA->_amc13_trailer_evt_lgth != eventB-> _amc13_trailer_evt_lgth ) errorCounter++;
	if(eventA->_amc13_trailer_crc16 != eventB-> _amc13_trailer_crc16 ) errorCounter++;
	if(eventA->_amc13_trailer_evt_stat != eventB-> _amc13_trailer_evt_stat ) errorCounter++;
	if(eventA->_amc13_trailer_tts != eventB-> _amc13_trailer_tts ) errorCounter++;
	if(eventA->_amc13_trailer_c != eventB-> _amc13_trailer_c ) {
		errorCounter++;
	}
	if(eventA->_amc13_trailer_f != eventB-> _amc13_trailer_f ) errorCounter++;
	if(eventA->_amc13_trailer_t != eventB-> _amc13_trailer_t ) errorCounter++;
	if(eventA->_amc13_trailer_r != eventB-> _amc13_trailer_r ) errorCounter++;

	temp = errorCounter;
	return temp;
}
