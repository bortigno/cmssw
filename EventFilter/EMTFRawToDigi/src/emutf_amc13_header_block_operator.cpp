#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc13_header_block_operator.h"
#include <iostream>
//----------------------------------------------------------------------
// mtf7::emutf_header_operator::emutf_amc13_header_block_operator( error_value *error_status, emutf_event *event_info ): emutf_block_operator( error_status, event_info ){}

//----------------------------------------------------------------------
const mtf7::word_64bit *mtf7::emutf_amc13_header_block_operator::unpack( const word_64bit *at_ptr ){
////std::cout << "Unpacking emutf_amc13_header_block_operator" << std::endl;

   if (*_error_status != mtf7::NO_ERROR) return 0;

   _buffer_start_ptr = at_ptr;

  if (at_ptr== 0){ *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - - 
  // process 1st 64-bit word ...
  // CDF header
  break_into_abcd_words( *at_ptr); at_ptr++;

  // check format identifiers
  if ( (_16bit_word_a & 0xf000) != 0x5000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_event_info -> _amc13_header_evt_ty = (_16bit_word_a >> 8) & 0xf;
  _unpacked_event_info -> _amc13_header_lv1_id = _16bit_word_b & 0xffff;
  _unpacked_event_info -> _amc13_header_lv1_id |= (_16bit_word_a & 0xff) << 16;
  _unpacked_event_info -> _amc13_header_bx_id = (_16bit_word_c >> 4) & 0xfff;
  _unpacked_event_info -> _amc13_header_source_id = (_16bit_word_c & 0xf) << 8;
  _unpacked_event_info -> _amc13_header_source_id |= (_16bit_word_d >> 8) & 0xff;
  _unpacked_event_info -> _amc13_header_x = (_16bit_word_d >> 2) & 0x1;
  _unpacked_event_info -> _amc13_header_h = (_16bit_word_d >> 3) & 0x1;
  _unpacked_event_info -> _amc13_header_fov = (_16bit_word_d >> 4) & 0xf;

  // - - - - - - - - - - - - - - - - - - - - - 
  // 2nd 64-bit word ...
  break_into_abcd_words( *at_ptr); at_ptr++;

  // set values derived from 1st word
_unpacked_event_info -> _amc13_header_namc  =  _16bit_word_a & 0xf; _16bit_word_a <<= 4;
_unpacked_event_info -> _amc13_header_res   =  _16bit_word_a & 0xf; _16bit_word_a <<= 4;
_unpacked_event_info -> _amc13_header_ufov  =  _16bit_word_a & 0xf;

  // next 16 bits are reserved and set to 0.

  _unpacked_event_info -> _amc13_header_orn = _16bit_word_b & 0xfff0;
  _unpacked_event_info -> _amc13_header_orn |= (_16bit_word_c & 0xffff) << 12;
  //_unpacked_event_info -> _amc13_header_orn |= (_16bit_word_a & 0xf) << 28;
  _unpacked_event_info -> _amc13_header_orn |= (_16bit_word_d & 0xf) << 28;

  return at_ptr;

}

unsigned long mtf7::emutf_amc13_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // pack 1st 64bit word --------------------
  // CDF header

  _16bit_word_a = _event_info_to_pack ->_amc13_header_evt_ty & 0xf; _16bit_word_a <<= 8;
  _16bit_word_a |= (_event_info_to_pack -> _amc13_header_lv1_id >> 16) & 0xff;
  _16bit_word_a |= 0x5000;

  _16bit_word_b = _event_info_to_pack -> _amc13_header_lv1_id & 0xffff;

  _16bit_word_c = _event_info_to_pack -> _amc13_header_bx_id & 0xfff; _16bit_word_c <<= 4;
  _16bit_word_c |= (_event_info_to_pack -> _amc13_header_source_id >> 8) & 0xf;

  _16bit_word_d = _event_info_to_pack -> _amc13_header_source_id & 0xff; _16bit_word_d <<= 4;
  _16bit_word_d |= _event_info_to_pack -> _amc13_header_fov & 0xf; _16bit_word_d <<= 1;
  _16bit_word_d |= _event_info_to_pack -> _amc13_header_h & 0x1; _16bit_word_d <<= 1;
  _16bit_word_d |= _event_info_to_pack -> _amc13_header_x & 0x1; _16bit_word_d <<= 2;


  *ptr = merge_abcd_words(); ptr++;

  // pack 2nd 64bit word --------------------

  _16bit_word_a = _event_info_to_pack -> _amc13_header_ufov & 0xf; _16bit_word_a <<= 4;
  _16bit_word_a |= _event_info_to_pack -> _amc13_header_res & 0xf; _16bit_word_a <<= 4;
  _16bit_word_a |= _event_info_to_pack -> _amc13_header_namc & 0xf; _16bit_word_a <<= 4;
  _16bit_word_a |= 0x0;

  _16bit_word_b = 0x0000;
  _16bit_word_b |= (_event_info_to_pack -> _amc13_header_orn >> 28) & 0xf;

  _16bit_word_c = (_event_info_to_pack -> _amc13_header_orn >> 12) & 0xffff;
  _16bit_word_d = _event_info_to_pack -> _amc13_header_orn & 0xfff; _16bit_word_d <<= 4;
  _16bit_word_d |= 0x0;


  *ptr = merge_abcd_words(); ptr++;
  

  return _nominal_buffer_size;

}
