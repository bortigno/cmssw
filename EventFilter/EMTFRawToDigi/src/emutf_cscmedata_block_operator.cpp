#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_cscmedata_block_operator.h"

const mtf7::word_64bit *mtf7::emutf_cscmedata_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_cscmedata_block_operator.cpp ######### ");
  
  // Pick the MTF7 ME data block
  emutf_cscmedata_block * _unpacked_block_event_info = new emutf_cscmedata_block(); 
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 64-bit word: ME
  break_into_dcba_words( *at_ptr ); at_ptr++;

  // Check that bit 15 of word_a and word_b = 0x1, and bit 15 of word_c and word_d = 0x0
  if ( ((_16bit_word_a >> 15) & 0x1) != 0x1 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_b >> 15) & 0x1) != 0x1 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_c >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_d >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  
  // If the block counter format is incorrect return the original pointer so you can attempt to unpack another pointer.
  if (*_error_status == mtf7::BLOCK_COUNTER_FORMAT) { at_ptr--; return at_ptr; } 
  else if (*_error_status != mtf7::NO_ERROR) { return 0; }

  _unpacked_block_event_info -> _csc_key_wire_group = (_16bit_word_a >> 8) & 0x7f; // Key wire group = bits 8 - 14 of word_a
  _unpacked_block_event_info -> _csc_quality        = (_16bit_word_a >> 4) & 0xf;  // Quality        = bits 4 -  7 of word_a
  _unpacked_block_event_info -> _csc_clct_pattern   = (_16bit_word_a & 0xf);       // CLCT pattern   = bits 0 -  3 of word_a

  _unpacked_block_event_info -> _csc_bc0                 = (_16bit_word_b >> 14) & 0x1; // BC0                 = bit      14 of word_b
  _unpacked_block_event_info -> _csc_bxe                 = (_16bit_word_b >> 13) & 0x1; // BXE                 = bit      13 of word_b
  _unpacked_block_event_info -> _csc_lr                  = (_16bit_word_b >> 12) & 0x1; // L/R                 = bit      12 of word_b
  _unpacked_block_event_info -> _csc_id                  = (_16bit_word_b >>  8) & 0xf; // CSC ID              = bits 8 - 11 of word_b
  _unpacked_block_event_info -> _csc_clct_key_half_strip =  _16bit_word_b & 0xff;       // CLCT key half-strip = bits 0 -  7 of word_b

  _unpacked_block_event_info -> _csc_afff   = (_16bit_word_c >> 14) & 0x1;  // AFFF   = bit      14 of word_c
  _unpacked_block_event_info -> _csc_cik    = (_16bit_word_c >> 13) & 0x1;  // CIK    = bit      13 of word_c
  _unpacked_block_event_info -> _csc_nit    = (_16bit_word_c >> 12) & 0x1;  // NIT    = bit      12 of word_c
  _unpacked_block_event_info -> _csc_me_bxn =  _16bit_word_c & 0xfff;       // ME BXN = bits 0 - 11 of word_c

  _unpacked_block_event_info -> _csc_afef     = (_16bit_word_d >> 14) & 0x1; // AFEF     = bit      14 of word_d
  _unpacked_block_event_info -> _csc_se       = (_16bit_word_d >> 13) & 0x1; // SE       = bit      13 of word_d
  _unpacked_block_event_info -> _csc_sm       = (_16bit_word_d >> 12) & 0x1; // SM       = bit      12 of word_d
  _unpacked_block_event_info -> _csc_epc      = (_16bit_word_d >>  8) & 0xf; // EPC      = bits 8 - 11 of word_d
  _unpacked_block_event_info -> _csc_af       = (_16bit_word_d >>  7) & 0x1; // AF       = bit       7 of word_d
  _unpacked_block_event_info -> _csc_station  = (_16bit_word_d >>  4) & 0x7; // Station  = bits 4 -  6 of word_d
  _unpacked_block_event_info -> _csc_vp       = (_16bit_word_d >>  3) & 0x1; // VP       = bit       3 of word_d
  _unpacked_block_event_info -> _csc_tbin_num =  _16bit_word_d & 0x7;        // TBIN Num = bits 0 -  2 of word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_cscmedata_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}


unsigned long mtf7::emutf_cscmedata_block_operator::pack(){

  // AWB 10.12.15 - TODO: check the emutf_cscmedata_block_operator packer with the latest format

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_cscmedata_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_cscmedata_block_vector.front();
  
  _16bit_word_a = _block_event_info_to_pack -> _csc_key_wire_group & 0x7f; _16bit_word_a <<= 4;
  _16bit_word_a |= _block_event_info_to_pack -> _csc_quality & 0xf; _16bit_word_a <<= 4;
  _16bit_word_a |= _block_event_info_to_pack -> _csc_quality & 0xf;
  _16bit_word_a |= 0x8000;

  _16bit_word_b = _block_event_info_to_pack -> _csc_bc0 & 0x1; _16bit_word_b <<= 1;
  _16bit_word_b |= _block_event_info_to_pack -> _csc_bxe & 0x1; _16bit_word_b <<= 1;
  _16bit_word_b |= _block_event_info_to_pack -> _csc_lr & 0x1; _16bit_word_b <<= 4;
  _16bit_word_b |= _block_event_info_to_pack -> _csc_id & 0xf; _16bit_word_b <<= 8;
  _16bit_word_b |= _block_event_info_to_pack -> _csc_clct_key_half_strip & 0xff;
  _16bit_word_b |= 0x8000;

  _16bit_word_c = _block_event_info_to_pack -> _csc_afff & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _csc_cik & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _csc_nit & 0x1; _16bit_word_c <<= 12;
  _16bit_word_c |= _block_event_info_to_pack -> _csc_me_bxn & 0xfff;

  _16bit_word_d = _block_event_info_to_pack -> _csc_afef & 0x1; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _csc_se & 0x1; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _csc_sm & 0x1; _16bit_word_d <<= 4;
  _16bit_word_d |= _block_event_info_to_pack -> _csc_epc & 0xf; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _csc_af & 0x1; _16bit_word_d <<=3;
  _16bit_word_d |= _block_event_info_to_pack -> _csc_station & 0x7; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _csc_vp & 0x1; _16bit_word_d <<= 3;
  _16bit_word_d |= _block_event_info_to_pack -> _csc_tbin_num & 0x7;

  *ptr = merge_dcba_words();

  return _nominal_buffer_size;


}
