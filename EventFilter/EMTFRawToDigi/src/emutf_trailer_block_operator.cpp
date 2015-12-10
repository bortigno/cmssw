#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_trailer_block_operator.h"

const mtf7::word_64bit *mtf7::emutf_trailer_block_operator::unpack( const word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_trailer_block_operator.cpp ######### ");

  // Pick the MTF7 event record trailer block
  emutf_trailer_block * _unpacked_block_event_info = new emutf_trailer_block(); 
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;

  _buffer_start_ptr = at_ptr;

  if (at_ptr== 0){ *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - - 
  // Unpack 1st 64-bit word: TR1
  break_into_dcba_words( *at_ptr); at_ptr++;

  // Check that bits 12 - 15 of all words = 0xF, bits 0 - 6 of word_b = 0x7F, and bits 9 - 11 of word_c equal 0x0
  if ( (_16bit_word_a & 0xf000) != 0xf000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT;
  if ( (_16bit_word_b & 0xf07f) != 0xf07f ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT;
  if ( (_16bit_word_c & 0xfe00) != 0xf000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT;
  if ( (_16bit_word_d & 0xf000) != 0xf000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT;

  // If the block counter format is incorrect return the original pointer so you can attempt to unpack another pointer.
  if (*_error_status == mtf7::BLOCK_COUNTER_FORMAT) { at_ptr--; return at_ptr; }
  else if (*_error_status != mtf7::NO_ERROR) { return 0; }

  _unpacked_block_event_info -> _trailer_ddcsr_lf  = (_16bit_word_a >> 8) & 0xf; // DD/CSR_LF  = bits 8 - 11 of word_a
  _unpacked_block_event_info -> _trailer_l1a       =  _16bit_word_a & 0xff;      // L1A        = bits 0 -  7 of word_a

  _unpacked_block_event_info -> _trailer_ddcsr_lf |= (_16bit_word_b >> 8) & 0xf; // DD/CSR_LF += bits 8 - 11 of word_b
  _unpacked_block_event_info -> _trailer_lfff      = (_16bit_word_b >> 7) & 0x1; // LFFF       = bit       7 of word_b

  _unpacked_block_event_info -> _trailer_ddcsr_bid = (_16bit_word_c & 0x1ff) << 7;  // DD/CSR_BID = bits 0 - 8 of word_c
  _unpacked_block_event_info -> _trailer_bb        = (_16bit_word_c >> 8) & 0x1;    // BB         = bit      8 of word_c
  _unpacked_block_event_info -> _trailer_yy        = (_16bit_word_c >> 4) & 0xf;    // YY         = bits 4 - 7 of word_c
  _unpacked_block_event_info -> _trailer_mm        =  _16bit_word_c & 0xf;          // MM         = bits 0 - 3 of word_c  

  _unpacked_block_event_info -> _trailer_spcsr_scc = _16bit_word_d & 0xfff; // SP/CSR_SCC = bits 0 - 11 of word_d

  // - - - - - - - - - - - - - - - - - - - - - 
  // Unpack 1st 64-bit word: TR2
  break_into_dcba_words( *at_ptr); at_ptr++;

  // Check that bits 12 - 15 of all words = 0xE, and bits 5 - 11 of word_a = 0x00
  if ( (_16bit_word_a & 0xffe0) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_b & 0xf000) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_c & 0xf000) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_d & 0xf000) != 0xe000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;

  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info -> _trailer_ddcsr_bid |=  _16bit_word_a & 0x1f; // DD/CSR_BID += bits 0 - 8 of word_a
  _unpacked_block_event_info -> _trailer_dd         =  _16bit_word_a & 0x1f; // DD          = bits 0 - 8 of word_a

  _unpacked_block_event_info -> _trailer_sp_ladr = (_16bit_word_b >> 8) & 0xf;  // SP LADR = bits 8 - 11 of word_b
  _unpacked_block_event_info -> _trailer_sp_ersv = (_16bit_word_b >> 5) & 0x7;  // SP ERS  = bits 5 -  7 of word_b
  _unpacked_block_event_info -> _trailer_sp_padr =  _16bit_word_b & 0x1f;        // SP PADR = bits 0 - 4 of word_b 
  
  _unpacked_block_event_info -> _trailer_lp    = (_16bit_word_c >> 11) & 0x1; // LP     = bit      11 of word_c
  _unpacked_block_event_info -> _trailer_crc22 =  _16bit_word_c & 0x7ff;      // CRC-22 = bits 0 - 10 of word_c

  _unpacked_block_event_info -> _trailer_hp    =  _16bit_word_d & 0x1;           // HP     = bit      11 of word_d
  _unpacked_block_event_info -> _trailer_crc22 = (_16bit_word_d & 0x7ff) << 11; // CRC-22 += bits 0 - 10 of word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_trailer_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}

unsigned long mtf7::emutf_trailer_block_operator::pack(){

  // AWB 10.12.15 - TODO: check the emutf_trailer_block_operator packer with the latest format

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_trailer_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_trailer_block_vector.front();

  // pack 1st 64bit word --------------------

  _16bit_word_a = _block_event_info_to_pack -> _trailer_ddcsr_lf & 0xf; _16bit_word_a <<= 8;
  _16bit_word_a |= _block_event_info_to_pack -> _trailer_l1a & 0xff;
  _16bit_word_a |= 0xf000;

  _16bit_word_b = _block_event_info_to_pack -> _trailer_ddcsr_lf & 0xf0; _16bit_word_b <<= 1;
  _16bit_word_b |= _block_event_info_to_pack -> _trailer_lfff & 0x1; _16bit_word_b <<= 7;
  _16bit_word_b |= 0x7f;
  _16bit_word_b |= 0xf000;

  _16bit_word_c = _block_event_info_to_pack -> _trailer_bb & 0x1; _16bit_word_c <<= 4;
  _16bit_word_c |= _block_event_info_to_pack -> _trailer_yy & 0xf; _16bit_word_c <<= 4;
  _16bit_word_c |= _block_event_info_to_pack -> _trailer_mm & 0xf;
  _16bit_word_c |= 0xf000;

  _16bit_word_d = _block_event_info_to_pack -> _trailer_spcsr_scc & 0x7ff;
  _16bit_word_d |= 0xf000;

  *ptr = merge_dcba_words(); ptr++;
  
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


  *ptr = merge_dcba_words(); ptr++;


  return _nominal_buffer_size;

}
