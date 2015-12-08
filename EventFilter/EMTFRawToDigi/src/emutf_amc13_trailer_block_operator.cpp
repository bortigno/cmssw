#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc13_trailer_block_operator.h"

//----------------------------------------------------------------------
// mtf7::emutf_trailer_operator::emutf_amc13_trailer_block_operator( error_value *error_status, emutf_event *event_info ): emutf_block_operator( error_status, event_info ){}

//----------------------------------------------------------------------
const mtf7::word_64bit *mtf7::emutf_amc13_trailer_block_operator::unpack( const word_64bit *at_ptr ){

   if (*_error_status != mtf7::NO_ERROR) return 0;

   _buffer_start_ptr = at_ptr;

  if (at_ptr== 0){ *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - - 
  // process 1st 64-bit word ...
  break_into_abcd_words( *at_ptr); at_ptr++;

  // check format identifiers
  if ( (_16bit_word_a & 0xf000) != 0xa000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  // pick the amc13 trailer block 
  emutf_amc13_trailer_block * _unpacked_block_event_info = _unpacked_event_info -> _emutf_amc13_trailer_block;

  // set values derived from 1st word
  _unpacked_block_event_info -> _amc13_trailer_evt_lgth = (_16bit_word_a & 0xff) << 16;
  _unpacked_block_event_info -> _amc13_trailer_evt_lgth |= _16bit_word_b & 0xffff;
  _unpacked_block_event_info -> _amc13_trailer_crc16 = _16bit_word_c & 0xffff;
  _16bit_word_d >>= 2;
  _unpacked_block_event_info -> _amc13_trailer_r = _16bit_word_d & 0x1; _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _amc13_trailer_t = _16bit_word_d & 0x1; _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _amc13_trailer_tts = _16bit_word_d & 0xf; _16bit_word_d >>= 4;
  _unpacked_block_event_info -> _amc13_trailer_evt_stat = _16bit_word_d & 0xf; _16bit_word_d >>= 6;
  _unpacked_block_event_info -> _amc13_trailer_f = _16bit_word_d & 0x1; _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _amc13_trailer_c = _16bit_word_d & 0x1;

  std::cout << "Event lenght : " << _unpacked_block_event_info -> _amc13_trailer_evt_lgth << std::endl;

  return at_ptr;

}

unsigned long mtf7::emutf_amc13_trailer_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // pick the amc13 triler block
  emutf_amc13_trailer_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_amc13_trailer_block;

  // pack 1st 64bit word --------------------

  _16bit_word_a = (_block_event_info_to_pack -> _amc13_trailer_evt_lgth >> 16) & 0xff;
  _16bit_word_a |= 0xa000;

  _16bit_word_b = _block_event_info_to_pack -> _amc13_trailer_evt_lgth & 0xffff;

  _16bit_word_c = _block_event_info_to_pack -> _amc13_trailer_crc16 & 0xffff;

  _16bit_word_d = _block_event_info_to_pack -> _amc13_trailer_c & 0x1; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_trailer_f & 0x1; _16bit_word_d <<= 6;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_trailer_evt_stat & 0xf; _16bit_word_d <<= 4;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_trailer_tts & 0xf; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_trailer_t & 0x1; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_trailer_r & 0x1; _16bit_word_d <<= 2;


  *ptr = merge_abcd_words(); ptr++;
  
  return _nominal_buffer_size;

}
