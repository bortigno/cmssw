#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc13_trailer_block_operator.h"

// Unpacks AMC13 trailer, 1 64-bit word, page 4/7, line 12 of docs/UpdatedDAQPath_2015-09-30.pdf
const mtf7::word_64bit *mtf7::emutf_amc13_trailer_block_operator::unpack( const word_64bit *at_ptr ){
  
  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_amc13_trailer_block_operator.cpp ######### ");

   if (*_error_status != mtf7::NO_ERROR) return 0;

   _buffer_start_ptr = at_ptr;

  if (at_ptr== 0){ *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - - 
  // Unpack 64-bit word: CDF Trailer
  break_into_abcd_words( *at_ptr); at_ptr++;

  // Check that bits 12 - 15 of word_a = 0xA
  if ( (_16bit_word_a & 0xf000) != 0xa000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  // Pick the AMC13 trailer block
  emutf_amc13_trailer_block * _unpacked_block_event_info = _unpacked_event_info -> _emutf_amc13_trailer_block;

  // Bits 8 - 11 of word_a are skipped
  _unpacked_block_event_info -> _amc13_trailer_evt_lgth  = (_16bit_word_a & 0xff) << 16; // Evt_lgth = bits 0 -  7 of word_a
  _unpacked_block_event_info -> _amc13_trailer_evt_lgth |=  _16bit_word_b & 0xffff;      //          + bits 0 - 15 of word_b
  _unpacked_block_event_info -> _amc13_trailer_crc16     =  _16bit_word_c & 0xffff;      // CRC      = bits 0 - 15 of word_c
  _unpacked_block_event_info -> _amc13_trailer_c         =  _16bit_word_d & 0x1;         // C        = bit      15 of word_d
  _unpacked_block_event_info -> _amc13_trailer_f         =  _16bit_word_d & 0x1;         // F        = bit      14 of word_d
  // Bits 12 - 13 of word_d are skipped
  _unpacked_block_event_info -> _amc13_trailer_evt_stat  = (_16bit_word_d >> 8) & 0xf;   // Evt_stat = bits 8 - 11 of word_d
  _unpacked_block_event_info -> _amc13_trailer_tts       = (_16bit_word_d >> 4) & 0xf;   // TTS      = bits 4 -  7 of word_d
  _unpacked_block_event_info -> _amc13_trailer_t         = (_16bit_word_d >> 3) & 0x1;   // T        = bit       3 of word_d
  _unpacked_block_event_info -> _amc13_trailer_r         = (_16bit_word_d >> 2) & 0x1;   // R        = bit       2 of word_d
  // Bits 0 - 1 of word_d are skipped

  return at_ptr;

}

unsigned long mtf7::emutf_amc13_trailer_block_operator::pack(){

  // AWB 10.12.15 - TODO: check the emutf_amc13_trailer_block_operator packer with the latest format

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
