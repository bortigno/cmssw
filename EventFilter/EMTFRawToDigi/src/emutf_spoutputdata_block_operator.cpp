#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_spoutputdata_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"
#include <bitset>


bool mtf7::emutf_spoutputdata_block_operator::isd15true ( mtf7::word_16bit word ){ return ( word & 0x8000 ); }

const mtf7::word_64bit *mtf7::emutf_spoutputdata_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

	// pick the spoutput data block
	emutf_spoutputdata_block * _unpacked_block_event_info; 
	_unpacked_block_event_info -> clear_block();

	if (*_error_status != mtf7::NO_ERROR) return 0;

	if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }


	// 1st 64 bit word
	break_into_abcd_words( *at_ptr ); at_ptr++;

	if (!isd15true(_16bit_word_a))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (isd15true(_16bit_word_b))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (!isd15true(_16bit_word_c))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (isd15true(_16bit_word_d))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (*_error_status != mtf7::NO_ERROR) return 0;

	_unpacked_block_event_info -> _track_phi_inner  = _16bit_word_a & 0xfff; _16bit_word_a >>= 12;
	_unpacked_block_event_info -> _track_vc = _16bit_word_a & 0x1; _16bit_word_a >>= 1;
	_unpacked_block_event_info -> _track_c = _16bit_word_a & 0x1; _16bit_word_a >>= 1;
	_unpacked_block_event_info -> _track_hl = _16bit_word_a & 0x1;

	_unpacked_block_event_info -> _track_phi_outer = _16bit_word_b & 0xfff; _16bit_word_b >>= 12;
	_unpacked_block_event_info -> _track_bc0 = _16bit_word_b & 0x1; _16bit_word_b >>= 1;
	_unpacked_block_event_info -> _track_se = _16bit_word_b & 0x1;

	_unpacked_block_event_info -> _track_eta = _16bit_word_c & 0x1ff; _16bit_word_c >>= 9;
	_unpacked_block_event_info -> _track_quality = _16bit_word_c & 0xf;

	_unpacked_block_event_info -> _track_pt = _16bit_word_d & 0x1ff; _16bit_word_d >>=9 ;
	_unpacked_block_event_info -> _track_bx = _16bit_word_d & 0x7; 

	// 2nd 64 bit word
	break_into_abcd_words( *at_ptr ); at_ptr++;

	if (isd15true(_16bit_word_a))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (!isd15true(_16bit_word_b))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (!isd15true(_16bit_word_c))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (isd15true(_16bit_word_d))  *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
	if (*_error_status != mtf7::NO_ERROR) return 0;

	_unpacked_block_event_info -> _track_me1_id = _16bit_word_a & 0x7; _16bit_word_a >>= 3;
	_unpacked_block_event_info -> _track_me2_id = _16bit_word_a & 0x3; _16bit_word_a >>= 2;
	_unpacked_block_event_info -> _track_me3_id = _16bit_word_a & 0x3; _16bit_word_a >>= 2;
	_unpacked_block_event_info -> _track_me4_id = _16bit_word_a & 0x3;

	_unpacked_block_event_info -> _track_me1_tbin = _16bit_word_b & 0x7; _16bit_word_b >>= 3;
	_unpacked_block_event_info -> _track_me2_tbin = _16bit_word_b & 0x7; _16bit_word_b >>= 3;
	_unpacked_block_event_info -> _track_me3_tbin = _16bit_word_b & 0x7; _16bit_word_b >>= 3;
	_unpacked_block_event_info -> _track_me4_tbin = _16bit_word_b & 0x7; _16bit_word_b >>= 3;
	_unpacked_block_event_info -> _track_tbin_num = _16bit_word_b & 0x7; 


	_unpacked_block_event_info -> _track_pt_lut_address = _16bit_word_c & 0x7fff;

	_unpacked_block_event_info -> _track_pt_lut_address |= (_16bit_word_d & 0x7fff) << 15;

	// now fill the vector of blocks in the event
	_unpacked_event_info -> _emutf_spoutputdata_block_vector.push_back(_unpacked_block_event_info);
		
	return at_ptr;

}


unsigned long mtf7::emutf_spoutputdata_block_operator::pack(){

	mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

	mtf7::word_64bit *ptr = buffer;

	// pick the block event info
	emutf_spoutputdata_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_spoutputdata_block->begin();

	// pack 1st 64 bit word

	_16bit_word_a = _block_event_info_to_pack -> _track_hl & 0x1; _16bit_word_a <<= 1;
	_16bit_word_a |= _block_event_info_to_pack -> _track_c & 0x1; _16bit_word_a <<= 1;
	_16bit_word_a |= _block_event_info_to_pack -> _track_vc & 0x1; _16bit_word_a <<= 12;
	_16bit_word_a |= _block_event_info_to_pack -> _track_phi_inner & 0xfff;
	_16bit_word_a |= 0x8000;

	_16bit_word_b = _block_event_info_to_pack -> _track_se & 0x1; _16bit_word_b <<= 1;
	_16bit_word_b |= _block_event_info_to_pack -> _track_bc0 & 0x1; _16bit_word_b <<= 12;
	_16bit_word_b |= _block_event_info_to_pack -> _track_phi_outer & 0xfff; 



	_16bit_word_c = _block_event_info_to_pack -> _track_quality & 0xf; _16bit_word_c <<= 9;
	_16bit_word_c |= _block_event_info_to_pack -> _track_eta & 0x1ff; 
	_16bit_word_c |= 0x8000;

	_16bit_word_d = _block_event_info_to_pack -> _track_bx & 0x7; _16bit_word_d <<= 9;
	_16bit_word_d |= _block_event_info_to_pack -> _track_pt & 0x1ff; 

	*ptr = merge_abcd_words(); ptr++;

	// pack 2nd 64 bit word

	_16bit_word_a = _block_event_info_to_pack -> _track_me4_id & 0x3; _16bit_word_a <<= 2;
	_16bit_word_a |= _block_event_info_to_pack -> _track_me3_id & 0x3; _16bit_word_a <<= 2;
	_16bit_word_a |= _block_event_info_to_pack -> _track_me2_id & 0x3; _16bit_word_a <<= 3;
	_16bit_word_a |= _block_event_info_to_pack -> _track_me1_id & 0x3;
	
	_16bit_word_b = _block_event_info_to_pack  -> _track_tbin_num & 0x7; _16bit_word_b <<= 3;
	_16bit_word_b |= _block_event_info_to_pack -> _track_me4_tbin & 0x7; _16bit_word_b <<= 3;
	_16bit_word_b |= _block_event_info_to_pack -> _track_me3_tbin & 0x7; _16bit_word_b <<= 3;
	_16bit_word_b |= _block_event_info_to_pack -> _track_me2_tbin & 0x7; _16bit_word_b <<= 3;
	_16bit_word_b |= _block_event_info_to_pack -> _track_me1_tbin & 0x7; 
	_16bit_word_b |= 0x8000;
	

	_16bit_word_c = _block_event_info_to_pack -> _track_pt_lut_address & 0x7fff;
	_16bit_word_c |= 0x8000;

	_16bit_word_d = (_block_event_info_to_pack -> _track_pt_lut_address >> 15) & 0x7fff;

	*ptr = merge_abcd_words();

	return _nominal_buffer_size;


}
