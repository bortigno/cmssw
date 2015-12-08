#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc_header_block_operator.h"

// Unpacks MTF7 header, 2 64-bit words, page 3/7, lines 1 - 2 of docs/UpdatedDAQPath_2015-09-30.pdf
const mtf7::word_64bit *mtf7::emutf_amc_header_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){
  
  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_amc_header_block_operator.cpp ######### ");

  // What determines the order of the lines with "_unpacked_block_event_info", "*_error_status", and "NULL_BUFFER_PTR"?
  // What is the difference between "BLOCK_COUNTER_FORMAT" and "EVENT_RECORD_FORMAT"? - AWB 08.12.15
  
  // Pick the MTF7 header block (What does this mean? -AWB 05.12.15)
  emutf_amc_header_block * _unpacked_block_event_info = new emutf_amc_header_block(); 
  _unpacked_block_event_info -> clear_block();
  
  if (*_error_status != mtf7::NO_ERROR) return 0;
  
  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 1st 64-bit word: Header H
  break_into_abcd_words( *at_ptr ); at_ptr++;

  // Check that bits 12 - 15 of word_a = 0x0
  if ( (_16bit_word_a & 0xf000) != 0x0000 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  // If the format is incorrect return the original pointer so you can attempt to unpack another pointer.
  // If the format is incorrect, why do we not simply return an error? - AWB 08.12.15
  if (*_error_status == mtf7::BLOCK_COUNTER_FORMAT) { 
    at_ptr--; 
    return at_ptr; } 
  else if (*_error_status != mtf7::NO_ERROR){
    return 0;
  }
  
  _unpacked_block_event_info -> _amc_header_amc_number   = (_16bit_word_a >> 8) & 0xf;   // AmcNo     = bits 8 - 11 of word_a
  _unpacked_block_event_info -> _amc_header_lv1_id       = (_16bit_word_a & 0xff) << 16; // LV1_id    = bits 0 -  7 of word_a 
  _unpacked_block_event_info -> _amc_header_lv1_id      |= _16bit_word_b & 0xffff;       //           + bits 0 - 15 of word_b
  _unpacked_block_event_info -> _amc_header_bx_id        = (_16bit_word_c >> 4) & 0xfff; // BX_id     = bits 4 - 15 of word_c
  _unpacked_block_event_info -> _amc_header_data_length  = (_16bit_word_c & 0xf) << 16;  // Data_lgth = bits 0 -  3 of word_c
  _unpacked_block_event_info -> _amc_header_data_length |= _16bit_word_d & 0xffff;       //           + bits 1 - 15 of word_d


  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 2nd 64-bit word: Header D
  break_into_abcd_words( *at_ptr ); at_ptr++;

  _unpacked_block_event_info -> _amc_header_user_id      = (_16bit_word_a & 0xffff) << 16; // User    = bits 0 - 15 of word_a
  _unpacked_block_event_info -> _amc_header_user_id     |= _16bit_word_b & 0xffff;         //         + bits 0 - 15 of word_b
  _unpacked_block_event_info -> _amc_header_orbit_number = _16bit_word_c & 0xffff;         // OrN     = bits 0 - 15 of word_c
  _unpacked_block_event_info -> _amc_header_board_id     = _16bit_word_d & 0xffff;         // BoardID = bits 0 - 15 of word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_amc_header_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;
}


unsigned long mtf7::emutf_amc_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // Pick the MTF7 header block (What does this mean? -AWB 05.12.15) 
  emutf_amc_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_amc_header_block_vector.front();

  // - - - - - - - - - - - - - - - - - - - - -
  // Pack 1st 64-bit word: Header H
  _16bit_word_a  = 0x0 << 12;
  _16bit_word_a |= (_block_event_info_to_pack -> _amc_header_amc_number & 0xf) << 8;
  _16bit_word_a |= (_block_event_info_to_pack -> _amc_header_lv1_id >> 16) & 0xff;

  _16bit_word_b  = _block_event_info_to_pack -> _amc_header_lv1_id & 0xffff;
  
  _16bit_word_c  = (_block_event_info_to_pack -> _amc_header_bx_id & 0xfff) << 4;
  _16bit_word_c |= (_block_event_info_to_pack -> _amc_header_data_length >> 16) & 0xf;

  _16bit_word_d  = _block_event_info_to_pack -> _amc_header_data_length & 0xffff;

  *ptr = merge_abcd_words(); ptr++;

  // - - - - - - - - - - - - - - - - - - - - -
  // Pack 2nd 64-bit word: Header D

  _16bit_word_a  = (_block_event_info_to_pack -> _amc_header_user_id >> 16) & 0xffff;
  _16bit_word_b  = _block_event_info_to_pack -> _amc_header_user_id & 0xffff;
  _16bit_word_c  = _block_event_info_to_pack -> _amc_header_orbit_number & 0xffff;
  _16bit_word_d  = _block_event_info_to_pack -> _amc_header_board_id & 0xffff;

  // Does it matter whether there is a ptr++ after the last instance of merge_abcd_words()?
  // In some .cpp files it appears, in some it doesn't. Is there some deeper reason? - AWB 08.12.15
  *ptr = merge_abcd_words(); ptr++;

  return _nominal_buffer_size;
}
