#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc_trailer_block_operator.h"

// Unpacks MTF7 trailer, 64-bit word, page 3/7, lines 4 of docs/UpdatedDAQPath_2015-09-30.pdf
const mtf7::word_64bit *mtf7::emutf_amc_trailer_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_amc_trailer_block_operator.cpp ######### ");

  // Pick the MTF7 trailer block
  emutf_amc_trailer_block * _unpacked_block_event_info = new emutf_amc_trailer_block(); 
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 64-bit word: Trailer T
  break_into_abcd_words( *at_ptr ); at_ptr++;

  // Check that bits 4 - 7 of word_c = 0x0
  if ( (_16bit_word_c & 0x00f0) != 0x0000 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info -> _amc_trailer_crc32        = (_16bit_word_a & 0xffff) << 16; // CRC-32    = bits 0 - 15 of word_a 
  _unpacked_block_event_info -> _amc_trailer_crc32       |=  _16bit_word_b & 0xffff;        //           + bits 0 - 15 of word_b
  _unpacked_block_event_info -> _amc_trailer_lv1_id       = (_16bit_word_c >> 8) & 0xff;    // LV1_id    = bits 8 - 15 of word_c
  _unpacked_block_event_info -> _amc_trailer_data_length  = (_16bit_word_c & 0xf) << 16;    // Data_lgth = bits 0 -  3 of word_c 
  _unpacked_block_event_info -> _amc_trailer_data_length |= (_16bit_word_d & 0xffff);       //           + bits 0 - 15 of word_d
  
  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_amc_trailer_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}


unsigned long mtf7::emutf_amc_trailer_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // Pick the MTF7 trailer block
  emutf_amc_trailer_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_amc_trailer_block_vector.front();

  // - - - - - - - - - - - - - - - - - - - - -
  // Pack 64-bit word: Trailer T
  _16bit_word_a  = (_block_event_info_to_pack -> _amc_trailer_crc32 >> 16) & 0xffff;
  _16bit_word_b  =  _block_event_info_to_pack -> _amc_trailer_crc32 & 0xffff;
  _16bit_word_c  = (_block_event_info_to_pack -> _amc_trailer_lv1_id & 0xff) << 8;
  _16bit_word_c |=  0x0 << 4;
  _16bit_word_c |= (_block_event_info_to_pack -> _amc_trailer_data_length >> 16) & 0xf;
  _16bit_word_d =   _block_event_info_to_pack -> _amc_trailer_data_length & 0xffff;

  *ptr = merge_abcd_words();

  return _nominal_buffer_size;

}
