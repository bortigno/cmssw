#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_counter_block_operator.h"

const mtf7::word_64bit *mtf7::emutf_counter_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){


  // Pick the MTF7 counter block
  emutf_counter_block * _unpacked_block_event_info = new emutf_counter_block(); 
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 64-bit word: Block of Counters
  break_into_dcba_words( *at_ptr ); at_ptr++;

  // Check that bit 15 of word_a, word_c, and word_d = 0x0, and bit 15 of word_b = 0x1
  if ( ((_16bit_word_a >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_b >> 15) & 0x1) != 0x1 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_c >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_d >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }

  // If the block counter format is incorrect return the original pointer so you can attempt to unpack another pointer.
  if (*_error_status == mtf7::BLOCK_COUNTER_FORMAT) { at_ptr--; return at_ptr; } 
  else if (*_error_status != mtf7::NO_ERROR) { return 0; }

  _unpacked_block_event_info -> _TC  = (_16bit_word_a & 0x7fff);       // TC = bits 0 - 14 of word_a
  _unpacked_block_event_info -> _TC |= (_16bit_word_b & 0x7fff) << 15; //    + bits 0 - 14 of word_b
  _unpacked_block_event_info -> _OC  = (_16bit_word_c & 0x7fff);       // OC = bits 0 - 14 of word_c
  _unpacked_block_event_info -> _OC |= (_16bit_word_d & 0x7fff) << 15; //    + bits 0 - 14 of word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_counter_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}


unsigned long mtf7::emutf_counter_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // Pick the MTF7 block event info
  emutf_counter_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_counter_block_vector.front();
  
  _16bit_word_a = 0x0000 | (_block_event_info_to_pack -> _TC & 0x7fff);
  _16bit_word_b = 0x8000 | ( (_block_event_info_to_pack -> _TC >> 15) & 0x7fff);
  _16bit_word_c = 0x0000 | (_block_event_info_to_pack -> _OC & 0x7fff);
  _16bit_word_d = 0x8000 | ( (_block_event_info_to_pack -> _OC >> 15) & 0x7fff);

  *ptr = merge_dcba_words();

  return _nominal_buffer_size;

}
