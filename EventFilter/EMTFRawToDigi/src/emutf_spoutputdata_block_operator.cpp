#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_spoutputdata_block_operator.h"

const mtf7::word_64bit *mtf7::emutf_spoutputdata_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_spoutputdata_block_operator.cpp ######### ");

  // Pick the MTF7 SP output data block
  emutf_spoutputdata_block * _unpacked_block_event_info = new emutf_spoutputdata_block(); 
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;
  
  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 1st 64-bit word: SP1
  break_into_dcba_words( *at_ptr ); at_ptr++;

  // Check that bit 15 of word_a and word_c = 0x1, bit 15 of word_b and word_d = 0x0, and bits 8 - 11 of word_b = 0x0
  // bool mtf7::block_operator::is_bit_true( mtf7::word_16bit word, int bit_num) defined in src/block_operator.cpp
  // Returns true if bit at bit_num is 0x1, false if it is 0x0
  if (!is_bit_true(_16bit_word_a, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
  if ( is_bit_true(_16bit_word_b, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
  if (!is_bit_true(_16bit_word_c, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
  if ( is_bit_true(_16bit_word_d, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
  if ( ((_16bit_word_b >> 8) & 0xf) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
    
  // If the block counter format is incorrect return the original pointer so you can attempt to unpack another pointer.
  if (*_error_status == mtf7::BLOCK_COUNTER_FORMAT) { at_ptr--; return at_ptr; }
  else if (*_error_status != mtf7::NO_ERROR) { return 0; }

  _unpacked_block_event_info -> _track_hl       = (_16bit_word_a >> 14) & 0x1; // HL       = bit      14 of word_a
  _unpacked_block_event_info -> _track_c        = (_16bit_word_a >> 13) & 0x1; // C        = bit      13 of word_a
  _unpacked_block_event_info -> _track_vc       = (_16bit_word_a >> 12) & 0x1; // CV       = bit      12 of word_a
  _unpacked_block_event_info -> _track_phi_full =  _16bit_word_a & 0xfff;      // Phi_full = bits 0 - 11 of word_a
  
  _unpacked_block_event_info -> _track_vt      = (_16bit_word_b >> 14) & 0x1; // VT      = bit     14 of word_b
  _unpacked_block_event_info -> _track_se      = (_16bit_word_b >> 13) & 0x1; // SE      = bit     13 of word_b
  _unpacked_block_event_info -> _track_bc0     = (_16bit_word_b >> 12) & 0x1; // BC0     = bit     12 of word_b
  _unpacked_block_event_info -> _track_phi_gmt =  _16bit_word_b & 0xff;       // Phi_GMT = bits 0 - 7 of word_b
  
  _unpacked_block_event_info -> _track_bx      = (_16bit_word_c >> 13) & 0x3; // BX      = bits 13 - 14 of word_c
  _unpacked_block_event_info -> _track_quality = (_16bit_word_c >> 9) & 0xf;  // Quality = bits  9 - 12 of word_c
  _unpacked_block_event_info -> _track_eta     =  _16bit_word_c & 0x1ff;      // Eta_GMT = bits  0 -  8 of word_c
  
  _unpacked_block_event_info -> _track_me1_id = (_16bit_word_d >> 9) & 0x3f; // ME1_ID = bits 9 - 14 of word_d
  _unpacked_block_event_info -> _track_pt     =  _16bit_word_d & 0x1ff;      // Pt     = bits 0 -  8 of word_d

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 2nd 64-bit word: SP2
  break_into_dcba_words( *at_ptr ); at_ptr++;

  // Check that bit 15 of word_a and word_d = 0x0, and bit 15 of word_b and word_c = 0x1
  if ( is_bit_true(_16bit_word_a, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
  if (!is_bit_true(_16bit_word_b, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
  if (!is_bit_true(_16bit_word_c, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
  if ( is_bit_true(_16bit_word_d, 15)) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; 
    
  // Return 0 because if the block was incorrect it should have been spotted already at the first word.
  if (*_error_status != mtf7::NO_ERROR) return 0; 

  _unpacked_block_event_info -> _track_me4_id = (_16bit_word_d >> 10) & 0x1f; // ME4_ID = bits 10 - 14 of word_a
  _unpacked_block_event_info -> _track_me3_id = (_16bit_word_d >>  5) & 0x1f; // ME3_ID = bits  5 -  9 of word_a
  _unpacked_block_event_info -> _track_me2_id = _16bit_word_d & 0x1f;         // ME2_ID = bits  0 -  4 of word_a
  
  _unpacked_block_event_info -> _track_tbin_num = (_16bit_word_c >> 12) & 0x7; // TBIN Num = bits 12 - 14 of word_b
  _unpacked_block_event_info -> _track_me4_tbin = (_16bit_word_c >>  9) & 0x7; // ME4_TBIN = bits  9 - 11 of word_b
  _unpacked_block_event_info -> _track_me3_tbin = (_16bit_word_c >>  6) & 0x7; // ME3_TBIN = bits  6 -  8 of word_b
  _unpacked_block_event_info -> _track_me2_tbin = (_16bit_word_c >>  3) & 0x7; // ME2_TBIN = bits  3 -  5 of word_b
  _unpacked_block_event_info -> _track_me1_tbin =  _16bit_word_c & 0x7;        // ME1_TBIN = bits  0 -  2 of word_b
  
  _unpacked_block_event_info -> _track_pt_lut_address  =  _16bit_word_b & 0x7fff;        // PT_LUT_address = bits 0 - 14 of word_c
  _unpacked_block_event_info -> _track_pt_lut_address |= (_16bit_word_a & 0x7fff) << 15; //                + bits 0 - 14 of word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_spoutputdata_block_vector.push_back(_unpacked_block_event_info);
  
  return at_ptr;

}


unsigned long mtf7::emutf_spoutputdata_block_operator::pack(){

  // AWB 10.12.15 - TODO: check the emutf_spoutputdata_block_operator packer with the latest format

	mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

	mtf7::word_64bit *ptr = buffer;

	// pick the block event info
	emutf_spoutputdata_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_spoutputdata_block_vector.front();

	// pack 1st 64 bit word

	_16bit_word_a = _block_event_info_to_pack -> _track_hl & 0x1; _16bit_word_a <<= 1;
	_16bit_word_a |= _block_event_info_to_pack -> _track_c & 0x1; _16bit_word_a <<= 1;
	_16bit_word_a |= _block_event_info_to_pack -> _track_vc & 0x1; _16bit_word_a <<= 12;
	_16bit_word_a |= _block_event_info_to_pack -> _track_phi_full & 0xfff;
	_16bit_word_a |= 0x8000;

	_16bit_word_b = _block_event_info_to_pack -> _track_se & 0x1; _16bit_word_b <<= 1;
	_16bit_word_b |= _block_event_info_to_pack -> _track_bc0 & 0x1; _16bit_word_b <<= 12;
	_16bit_word_b |= _block_event_info_to_pack -> _track_phi_gmt & 0xfff; 



	_16bit_word_c = _block_event_info_to_pack -> _track_quality & 0xf; _16bit_word_c <<= 9;
	_16bit_word_c |= _block_event_info_to_pack -> _track_eta & 0x1ff; 
	_16bit_word_c |= 0x8000;

	_16bit_word_d = _block_event_info_to_pack -> _track_bx & 0x7; _16bit_word_d <<= 9;
	_16bit_word_d |= _block_event_info_to_pack -> _track_pt & 0x1ff; 

	*ptr = merge_dcba_words(); ptr++;

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

	*ptr = merge_dcba_words();

	return _nominal_buffer_size;


}
