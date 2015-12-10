#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_header_block_operator.h"

// Unpacks MTF7 event record header, 3 64-bit words, page 2/8, lines 1 - 12 of docs/EMU_TF_DAQ_format.pdf
const mtf7::word_64bit *mtf7::emutf_header_block_operator::unpack( const word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_header_block_operator.cpp ######### ");

  // Pick the MTF7 event record header block
  emutf_header_block * _unpacked_block_event_info = new emutf_header_block(); 
  _unpacked_block_event_info -> clear_block();
  
  if (*_error_status != mtf7::NO_ERROR) return 0;
  
  _buffer_start_ptr = at_ptr;

  if (at_ptr== 0){ *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 1st 64-bit word: HD1
  // Use "dcba" instead of "abcd" because this is the MTF7 format specified in docs/EMU_TF_DAQ_format.pdf, page 2:
  // word_a = bits 0 - 15, word_b = bits 16 - 31, word_c = bits 32 - 47, and word_d = bits 48 - 63
  break_into_dcba_words( *at_ptr); at_ptr++;

  // Check that bits 12 - 15 of word_a, word_b, and word_d = 0x9, and that word_c = 0x9000
  if ( (_16bit_word_a & 0xf000) != 0x9000 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( (_16bit_word_b & 0xf000) != 0x9000 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if (  _16bit_word_c           != 0x9000 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( (_16bit_word_d & 0xf000) != 0x9000 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }

  // If the format is incorrect return the original pointer so you can attempt to unpack another pointer.
  if (*_error_status == mtf7::BLOCK_COUNTER_FORMAT) { at_ptr--; return at_ptr; } 
  else if (*_error_status != mtf7::NO_ERROR) { return 0; }

  _unpacked_block_event_info -> _l1a  =  _16bit_word_a & 0xfff;        // L1A     = bits 0 - 11 of word_a
  _unpacked_block_event_info -> _l1a |= (_16bit_word_b & 0xfff) << 12; //         + bits 0 - 11 of word_b
  _unpacked_block_event_info -> _bxn  =  _16bit_word_d & 0xfff;        // L1A BXN = bits 0 - 11 of word_d

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 2nd 64-bit word: HD2
  break_into_dcba_words( *at_ptr); at_ptr++;

  // Check that word_a = 0xA000, that bits 12 - 15 of word_b, word_c, and word_d = 0xA, and that bit 11 of word_c = 0
  if (   _16bit_word_a            != 0xa000 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if (  (_16bit_word_b & 0xf000)  != 0xa000 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if (  (_16bit_word_c & 0xf000)  != 0xa000 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if ( ((_16bit_word_c >> 11) & 0x1) != 0x0 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  // if (  (_16bit_word_c & 0x800 )  !=  0x000 )  { *_error_status = mtf7::EVENT_RECORD_FORMAT; }  // Alternate expression; above is easier
  if (  (_16bit_word_d & 0xf000)  != 0xa000 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }

  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info -> _sp_ts   = (_16bit_word_b >> 8) & 0xf; // SP_TS   = bits 8 - 11 of word_b
  _unpacked_block_event_info -> _sp_ersv = (_16bit_word_b >> 5) & 0x7; // SP_ERSV = bits 5 -  7 of word_b
  _unpacked_block_event_info -> _sp_addr =  _16bit_word_b & 0x1f;      // SP_ADDR = bits 0 -  4 of word_b

  // IKF todo: size of at_ptr from _tbin and _skip
  _unpacked_block_event_info -> _tbin    = (_16bit_word_c >> 8) & 0x7; // TBIN = bits 8 - 10 of word_c 
  _unpacked_block_event_info -> _ddm     = (_16bit_word_c >> 7) & 0x1; // ddm  = bit       7 of word_c
  _unpacked_block_event_info -> _spa     = (_16bit_word_c >> 6) & 0x1; // spa  = bit       6 of word_c
  _unpacked_block_event_info -> _rpca    = (_16bit_word_c >> 5) & 0x1; // rpca = bit       5 of word_c 
  _unpacked_block_event_info -> _skip    = (_16bit_word_c >> 4) & 0x1; // skip = bit       4 of word_c
  _unpacked_block_event_info -> _rdy     = (_16bit_word_c >> 3) & 0x1; // rdy  = bit       3 of word_c
  _unpacked_block_event_info -> _bsy     = (_16bit_word_c >> 2) & 0x1; // bsy  = bit       2 of word_c
  _unpacked_block_event_info -> _osy     = (_16bit_word_c >> 1) & 0x1; // osy  = bit       1 of word_c
  _unpacked_block_event_info -> _wof     =  _16bit_word_c & 0x1;       // wof  = bit       0 of word_c
  
  _unpacked_block_event_info -> _ME1a    = _16bit_word_d & 0xfff;      // ME1a = bits 0 - 11 of word_d

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 3rd 64-bit word: HD3
  break_into_dcba_words( *at_ptr); at_ptr++;

  // Check that bit 15 of word_a = 0x1 and that bit 15 of word_b, word_c, and word_d = 0x0
  if ( ((_16bit_word_a >> 15) & 0x1) != 0x1 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if ( ((_16bit_word_b >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if ( ((_16bit_word_c >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if ( ((_16bit_word_d >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }

  if (*_error_status != mtf7::NO_ERROR) return 0;

  // Bits 9 - 14 of word_a, and 11 - 14 of word_b, word_c, and word_d, are unassigned
  _unpacked_block_event_info -> _ME1b =  _16bit_word_a & 0x1ff; // ME1b = bits 0 -  8 of word_a 
  _unpacked_block_event_info -> _ME2  =  _16bit_word_b & 0x4ff; // ME2  = bits 0 - 10 of word_b
  _unpacked_block_event_info -> _ME3  =  _16bit_word_c & 0x4ff; // ME3  = bits 0 - 10 of word_c
  _unpacked_block_event_info -> _ME4  =  _16bit_word_d & 0x4ff; // ME4  = bits 0 - 10 of word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_header_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}

unsigned long mtf7::emutf_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // Pick the MTF7 event record header block (What does this mean? -AWB 05.12.15)
  emutf_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_header_block_vector.front();

  // - - - - - - - - - - - - - - - - - - - - -
  // Pack 1st 64-bit word: HD1
  // Use "dcba" instead of "abcd" because this is the MTF7 format specified in docs/EMU_TF_DAQ_format.pdf, page 2:
  // word_a = bits 0 - 15, word_b = bits 16 - 31, word_c = bits 32 - 47, and word_d = bits 48 - 63
  _16bit_word_a = 0x9000 | (_block_event_info_to_pack -> _l1a & 0xfff);
  _16bit_word_b = 0x9000 | ( (_block_event_info_to_pack -> _l1a >> 12) & 0xfff);
  _16bit_word_c = 0x9000;
  _16bit_word_d = 0x9000 | (_block_event_info_to_pack -> _bxn & 0xfff);

  *ptr = merge_dcba_words(); ptr++;

  // - - - - - - - - - - - - - - - - - - - - -
  // Pack 2nd 64-bit word: HD2
  _16bit_word_a  =  0xa000;

  _16bit_word_b  =  0xa000;
  _16bit_word_b |= (_block_event_info_to_pack -> _sp_ts & 0xf) << 8;
  _16bit_word_b |= (_block_event_info_to_pack -> _sp_ersv & 0x7) << 5;
  _16bit_word_b |=  _block_event_info_to_pack -> _sp_addr & 0x1f;

  _16bit_word_c  =  0xa000;
  _16bit_word_c |= (_block_event_info_to_pack -> _tbin & 0x7) << 8;
  _16bit_word_c |= (_block_event_info_to_pack -> _ddm & 0x1) << 7;
  _16bit_word_c |= (_block_event_info_to_pack -> _spa & 0x1) << 6;
  _16bit_word_c |= (_block_event_info_to_pack -> _rpca & 0x1) << 5;
  _16bit_word_c |= (_block_event_info_to_pack -> _skip & 0x1) << 4;
  _16bit_word_c |= (_block_event_info_to_pack -> _rdy & 0x1) << 3;
  _16bit_word_c |= (_block_event_info_to_pack -> _bsy & 0x1) << 2;
  _16bit_word_c |= (_block_event_info_to_pack -> _osy & 0x1) << 1;
  _16bit_word_c |= (_block_event_info_to_pack -> _wof & 0x1); 

  _16bit_word_d  =  0xa000;
  _16bit_word_d |=  _block_event_info_to_pack -> _ME1a & 0xfff;

  *ptr = merge_dcba_words(); ptr++;

  // - - - - - - - - - - - - - - - - - - - - -
  // Pack 3rd 64-bit word: HD3
  _16bit_word_a = 0x1000 | (_block_event_info_to_pack -> _ME1b & 0x1ff);
  _16bit_word_b = 0x0000 | (_block_event_info_to_pack -> _ME2 & 0x4ff);
  _16bit_word_c = 0x0000 | (_block_event_info_to_pack -> _ME3 & 0x4ff);
  _16bit_word_d = 0x0000 | (_block_event_info_to_pack -> _ME4 & 0x4ff);

  *ptr = merge_dcba_words(); ptr++;

  return _nominal_buffer_size;

}
