#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_postpayload_trailer_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"

const mtf7::word_64bit *mtf7::emutf_postpayload_trailer_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  // pick the postpayload header data block
  emutf_postpayload_trailer_block & _unpacked_block_event_info = *(_unpacked_event_info -> _emutf_postpayload_trailer_block);

//std::cout << "Unpacking PrePayloadHeader block" << std::endl;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  break_into_abcd_words( *at_ptr ); at_ptr++;
  if ( (_16bit_word_c & 0x8000 ) != 0x0000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D15 is 0 
  if ( (_16bit_word_c & 0x4000 ) != 0x0000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D14 is 0
  if ( (_16bit_word_c & 0x2000 ) != 0x0000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D13 is 0
  if ( (_16bit_word_c & 0x1000 ) != 0x0000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D12 is 0
 
  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info . _postpayload_trailer_bxid = (_16bit_word_d & 0xfff); 

  _unpacked_block_event_info . _postpayload_trailer_lv1_id = (_16bit_word_d & 0xf000);
  _unpacked_block_event_info . _postpayload_trailer_lv1_id |= (_16bit_word_c & 0x000f) << 4;

  _unpacked_block_event_info . _postpayload_trailer_block_number = (_16bit_word_c & 0x0ff0);

  _unpacked_block_event_info . _postpayload_trailer_crc32 = (_16bit_word_b & 0xffff);
  _unpacked_block_event_info . _postpayload_trailer_crc32 |= (_16bit_word_a & 0xffff) << 16;

  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _postpayload_trailer_bxid );
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _postpayload_trailer_lv1_id);
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _postpayload_trailer_block_number );
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _postpayload_trailer_crc32);

  return at_ptr;
}


unsigned long mtf7::emutf_postpayload_trailer_block_operator::pack(){

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