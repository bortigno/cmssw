#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_trailer_block_operator.h"

//----------------------------------------------------------------------
// mtf7::emutf_trailer_operator::emutf_trailer_block_operator( error_value *error_status, emutf_event *event_info ): emutf_block_operator( error_status, event_info ){}

//----------------------------------------------------------------------
const mtf7::word_64bit *mtf7::emutf_trailer_block_operator::unpack( const word_64bit *at_ptr ){

  // pick the spoutput data block
  emutf_trailer_block * _unpacked_block_event_info = new emutf_trailer_block(); 
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;

  _buffer_start_ptr = at_ptr;

  if (at_ptr== 0){ *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - - 
  // process 1st 64-bit word ...
  break_into_abcd_words( *at_ptr); at_ptr++;

  // check format identifiers
  if ( (_16bit_word_a & 0xf000) != 0xf000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_b & 0xf07f) != 0xf07f ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_c & 0xf000) != 0xf000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_c & 0xe00 ) != 0x000  ) *_error_status = mtf7::EVENT_RECORD_FORMAT; //BAM
  if ( (_16bit_word_d & 0xf000) != 0xf000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  // set values derived from 1st word
  _unpacked_block_event_info -> _trailer_l1a  =  _16bit_word_a & 0xff; _16bit_word_a >>= 8;
  _unpacked_block_event_info -> _trailer_ddcrs_lf = _16bit_word_a & 0xf;
  _16bit_word_b >>= 7;
  _unpacked_block_event_info -> _trailer_lfff = _16bit_word_b & 0x1; _16bit_word_b >>= 1;
  _unpacked_block_event_info -> _trailer_ddcrs_lf |= (_16bit_word_b & 0xf) << 4;
  _unpacked_block_event_info -> _trailer_mm = _16bit_word_c & 0xf; _16bit_word_c >>= 4;
  _unpacked_block_event_info -> _trailer_yy = _16bit_word_c & 0xf; _16bit_word_c >>= 4;
  _unpacked_block_event_info -> _trailer_bb = _16bit_word_c & 0x1;
  _unpacked_block_event_info -> _trailer_spcrs_scc = _16bit_word_d & 0xfff;

  // - - - - - - - - - - - - - - - - - - - - - 
  // 2nd 64-bit word ...
  break_into_abcd_words( *at_ptr); at_ptr++;

  // check format identifiers
  if ( (_16bit_word_a & 0xffe0) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_b & 0xf000) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_c & 0xf000) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_d & 0xf000) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;

  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info -> _trailer_dd = _16bit_word_a & 0x1f;

  _unpacked_block_event_info -> _trailer_sp_padr = _16bit_word_b & 0x1f; _16bit_word_b >>= 5;
  _unpacked_block_event_info -> _trailer_sp_ersv = _16bit_word_b & 0x7; _16bit_word_b >>= 3;
  _unpacked_block_event_info -> _trailer_sp_ladr = _16bit_word_b & 0xf;
  
  _unpacked_block_event_info -> _trailer_crc22 = _16bit_word_c & 0x7ff; _16bit_word_c >>= 11;
  _unpacked_block_event_info -> _trailer_lp = _16bit_word_c & 0x1;

  _unpacked_block_event_info -> _trailer_crc22 = (_16bit_word_d & 0x7ff) << 11; _16bit_word_d >>= 11;
  _unpacked_block_event_info -> _trailer_hp = _16bit_word_d & 0x1;

  // now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_trailer_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}

unsigned long mtf7::emutf_trailer_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_trailer_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_trailer_block_vector.front();

  // pack 1st 64bit word --------------------

  _16bit_word_a = _block_event_info_to_pack -> _trailer_ddcrs_lf & 0xf; _16bit_word_a <<= 8;
  _16bit_word_a |= _block_event_info_to_pack -> _trailer_l1a & 0xff;
  _16bit_word_a |= 0xf000;

  _16bit_word_b = _block_event_info_to_pack -> _trailer_ddcrs_lf & 0xf0; _16bit_word_b <<= 1;
  _16bit_word_b |= _block_event_info_to_pack -> _trailer_lfff & 0x1; _16bit_word_b <<= 7;
  _16bit_word_b |= 0x7f;
  _16bit_word_b |= 0xf000;

  _16bit_word_c = _block_event_info_to_pack -> _trailer_bb & 0x1; _16bit_word_c <<= 4;
  _16bit_word_c |= _block_event_info_to_pack -> _trailer_yy & 0xf; _16bit_word_c <<= 4;
  _16bit_word_c |= _block_event_info_to_pack -> _trailer_mm & 0xf;
  _16bit_word_c |= 0xf000;

  _16bit_word_d = _block_event_info_to_pack -> _trailer_spcrs_scc & 0x7ff;
  _16bit_word_d |= 0xf000;

  *ptr = merge_abcd_words(); ptr++;
  
  // pack 2nd 64bit word --------------------

  _16bit_word_a = _block_event_info_to_pack -> _trailer_dd & 0x1f;
  _16bit_word_a |= 0xe000;

  _16bit_word_b = _block_event_info_to_pack -> _trailer_sp_ladr & 0xf; _16bit_word_b <<= 3;
  _16bit_word_b |= _block_event_info_to_pack -> _trailer_sp_ersv & 0x7; _16bit_word_b <<= 5;
  _16bit_word_b |= _block_event_info_to_pack -> _trailer_sp_padr & 0xf;
  _16bit_word_b |= 0xe000;

  _16bit_word_c = _block_event_info_to_pack -> _trailer_lp & 0x1; _16bit_word_c <<= 11;
  _16bit_word_c |= _block_event_info_to_pack -> _trailer_crc22 & 0x7ff;
  _16bit_word_c |= 0xe000;

  _16bit_word_d = _block_event_info_to_pack -> _trailer_hp & 0x1; _16bit_word_d <<= 11;
  _16bit_word_d |= ( (_block_event_info_to_pack -> _trailer_crc22) >> 11 ) & 0x7ff;
  _16bit_word_d |= 0xe000;


  *ptr = merge_abcd_words(); ptr++;


  return _nominal_buffer_size;

}
