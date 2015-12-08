#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_prepayload_header_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"

// Unpacks AMC13 header information about the MTF7 payloads, 1 64-bit word per input MTF7, page 4/7, lines 3 - 6 of docs/UpdatedDAQPath_2015-09-30.pdf
const mtf7::word_64bit *mtf7::emutf_prepayload_header_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_prepayload_header_block_operator.cpp ######### ");


  // Pick the pre-payload header block (What does this mean? -AWB 05.12.15)
  emutf_prepayload_header_block * _unpacked_block_event_info = new emutf_prepayload_header_block();
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 1 64-bit word: LMSEPVC + MTF7 payload size
  break_into_abcd_words( *at_ptr ); at_ptr++;

  // word_a bit 15 is empty in docs/UpdatedDAQPath_2015-09-30.pdf
  _unpacked_block_event_info -> _prepayload_l          = (_16bit_word_a >> 14) & 0x1;  // L        = bit      14 of word_a
  _unpacked_block_event_info -> _prepayload_m          = (_16bit_word_a >> 13) & 0x1;  // M        = bit      13 of word_a
  _unpacked_block_event_info -> _prepayload_s          = (_16bit_word_a >> 12) & 0x1;  // S        = bit      12 of word_a
  _unpacked_block_event_info -> _prepayload_e          = (_16bit_word_a >> 11) & 0x1;  // E        = bit      11 of word_a
  _unpacked_block_event_info -> _prepayload_p          = (_16bit_word_a >> 10) & 0x1;  // P        = bit      10 of word_a
  _unpacked_block_event_info -> _prepayload_v          = (_16bit_word_a >> 9) & 0x1;   // V        = bit       9 of word_a
  _unpacked_block_event_info -> _prepayload_c          = (_16bit_word_a >> 8) & 0x1;   // C        = bit       8 of word_a  
  _unpacked_block_event_info -> _prepayload_amcn_size  = (_16bit_word_a & 0xff) << 16; // AMC_size = bits 0 -  7 of word_a
  _unpacked_block_event_info -> _prepayload_amcn_size |= _16bit_word_b & 0xffff;       //          + bits 0 - 15 of word_b  

  // Check that bits 12 - 15 of word_c = 0x0
  if ( (_16bit_word_c & 0xf000) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }

  _unpacked_block_event_info -> _prepayload_blkNo      = (_16bit_word_c >> 4) & 0xff; // Blk_No    = bits 4 - 11 of word_c 
  _unpacked_block_event_info -> _prepayload_amcno      = _16bit_word_c & 0xf;         // AmcNo     = bits 0 -  3 of word_c
  _unpacked_block_event_info -> _prepayload_boardid    = _16bit_word_d & 0xffff;      // BoardID   = bits 0 - 15 od word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_prepayload_header_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;
}



unsigned long mtf7::emutf_prepayload_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // Pick the block event info (What does this mean? -AWB 05.12.15)
  emutf_prepayload_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_prepayload_header_block_vector.front();


  // - - - - - - - - - - - - - - - - - - - - -
  // Pack 1 64-bit word: LMSEPVC + MTF7 payload size
  // Why don't we fill bit 15 (empty in docs/UpdatedDAQPath_2015-09-30.pdf) here? - AWB 08.12.15
  _16bit_word_a  = (_block_event_info_to_pack -> _prepayload_l & 0x1) << 14;
  _16bit_word_a |= (_block_event_info_to_pack -> _prepayload_m & 0x1) << 13;
  _16bit_word_a |= (_block_event_info_to_pack -> _prepayload_s & 0x1) << 12;
  _16bit_word_a |= (_block_event_info_to_pack -> _prepayload_e & 0x1) << 11;
  _16bit_word_a |= (_block_event_info_to_pack -> _prepayload_p & 0x1) << 10;
  _16bit_word_a |= (_block_event_info_to_pack -> _prepayload_v & 0x1) <<  9;
  _16bit_word_a |= (_block_event_info_to_pack -> _prepayload_c & 0x1) <<  8;
  _16bit_word_a |= (_block_event_info_to_pack -> _prepayload_amcn_size >> 16) & 0xff;

  _16bit_word_b  = _block_event_info_to_pack -> _prepayload_amcn_size & 0xffff;

  _16bit_word_c  = 0x0 << 12;
  _16bit_word_c |= (_block_event_info_to_pack -> _prepayload_blkNo & 0xff) << 4;
  _16bit_word_c |= _block_event_info_to_pack -> _prepayload_amcno & 0xf;

  _16bit_word_d = _block_event_info_to_pack -> _prepayload_boardid & 0xffff;

  *ptr = merge_abcd_words();

  return _nominal_buffer_size;
}
