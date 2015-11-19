#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_counter_block_operator.h"

const mtf7::word_64bit *mtf7::emutf_counter_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){
//std::cout << "Unpacking emutf_counter_block_operator" << std::endl;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  break_into_abcd_words( *at_ptr ); at_ptr++;
  
  if ( (_16bit_word_a & 0x8000) != 0x0000 )            *_error_status = mtf7::BLOCK_COUNTER_FORMAT;// check if D15 is 0 
  if ( (_16bit_word_b & 0x8000) != 0x8000 )            *_error_status = mtf7::BLOCK_COUNTER_FORMAT;// check if D15 is 1
  if ( (_16bit_word_c & 0x8000) != 0x0000 )            *_error_status = mtf7::BLOCK_COUNTER_FORMAT;// check if D15 is 0
  if ( (_16bit_word_d & 0x8000) != 0x0000 )            *_error_status = mtf7::BLOCK_COUNTER_FORMAT;// check if D15 is 0
  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_event_info -> _TC  = (_16bit_word_a & 0x7fff);
  _unpacked_event_info -> _TC |= (_16bit_word_b & 0x7fff) << 15;

  _unpacked_event_info -> _OC  = (_16bit_word_c & 0x7fff);
  _unpacked_event_info -> _OC |= (_16bit_word_d & 0x7fff) << 15;

  return at_ptr;

}


unsigned long mtf7::emutf_counter_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;
  
  mtf7::word_32bit tmp_32bit_word = _event_info_to_pack -> _TC;

  _16bit_word_a = 0x0000 | (tmp_32bit_word & 0x7fff); tmp_32bit_word >>= 15;
  _16bit_word_b = 0x8000 | (tmp_32bit_word & 0x7fff); 

  tmp_32bit_word = _event_info_to_pack -> _OC;

  _16bit_word_c = tmp_32bit_word & 0x7fff; tmp_32bit_word >>= 15;
  _16bit_word_d = tmp_32bit_word & 0x7fff;

  *ptr = merge_abcd_words();

  return _nominal_buffer_size;


}
