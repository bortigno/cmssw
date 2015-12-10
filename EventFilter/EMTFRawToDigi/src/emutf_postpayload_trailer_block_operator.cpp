#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_postpayload_trailer_block_operator.h"

// Unpacks AMC13 trailer information about the MTF7 payloads, 1 64-bit word per input MTF7, page 4/7, line 11 of docs/UpdatedDAQPath_2015-09-30.pdf
const mtf7::word_64bit *mtf7::emutf_postpayload_trailer_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_postpayload_trailer_block_operator.cpp ######### ");

  // Pick the post-payload header block
  emutf_postpayload_trailer_block & _unpacked_block_event_info = *(_unpacked_event_info -> _emutf_postpayload_trailer_block);

  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 64-bit word: CRC-32, Blk_No, LV1_id, and BX_id
  break_into_abcd_words( *at_ptr ); at_ptr++;

  // Check that bits 12 - 15 of word_c = 0x0
  if ( (_16bit_word_c & 0xf000) != 0x0000 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info . _postpayload_trailer_crc32        = (_16bit_word_a & 0xffff) << 16; // CRC-32 = bits  0 - 15 of word_a
  _unpacked_block_event_info . _postpayload_trailer_crc32       |= (_16bit_word_b & 0xffff);       //        + bits  0 - 15 of word_b
  _unpacked_block_event_info . _postpayload_trailer_block_number = (_16bit_word_c >> 4) & 0xff;    // Blk_No = bits  4 - 11 of word_c
  _unpacked_block_event_info . _postpayload_trailer_lv1_id       = (_16bit_word_c & 0xf) << 4;     // LV1_id = bits  0 -  3 of word_c
  _unpacked_block_event_info . _postpayload_trailer_lv1_id      |= (_16bit_word_d >> 12) & 0xf;    //        + bits 12 - 15 of word_d
  _unpacked_block_event_info . _postpayload_trailer_bxid         = (_16bit_word_d & 0xfff);        // BX_id  = bits  0 - 12 of word_d
  
  return at_ptr;
}


unsigned long mtf7::emutf_postpayload_trailer_block_operator::pack(){

  // AWB 10.12.15 - TODO: check the emutf_postpayload_trailer_block_operator packer with the latest format

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_postpayload_trailer_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_postpayload_trailer_block;

  _16bit_word_d = _block_event_info_to_pack -> _postpayload_trailer_lv1_id & 0x00ff; _16bit_word_d <<= 12; 
  _16bit_word_d |= _block_event_info_to_pack -> _postpayload_trailer_bxid & 0xffff;

  _16bit_word_c = _block_event_info_to_pack -> _postpayload_trailer_block_number & 0xffff; _16bit_word_c <<= 4;
  _16bit_word_c |= _block_event_info_to_pack -> _postpayload_trailer_lv1_id & 0xff00;

  mtf7::word_32bit tmp_32bit_word = _block_event_info_to_pack -> _postpayload_trailer_crc32;

  _16bit_word_b = tmp_32bit_word & 0xffff; tmp_32bit_word >>=16;
  _16bit_word_a = tmp_32bit_word & 0xffff;  

  *ptr = merge_abcd_words();

  return _nominal_buffer_size;
}
