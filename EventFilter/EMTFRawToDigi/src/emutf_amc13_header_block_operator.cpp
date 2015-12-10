#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc13_header_block_operator.h"

// Unpacks AMC13 header, 2 64-bit words, page 4/7, lines 1 - 2 of docs/UpdatedDAQPath_2015-09-30.pdf
const mtf7::word_64bit *mtf7::emutf_amc13_header_block_operator::unpack( const word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_amc13_header_block_operator.cpp ######### ");
  
  if (*_error_status != mtf7::NO_ERROR) return 0;
   
  _buffer_start_ptr = at_ptr;
   
  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - - 
  // Unpack 1st 64-bit word: CDF Header
  break_into_abcd_words( *at_ptr ); at_ptr++;
  
  // Check that bits 12 - 15 of word_a = 0x5
  // Bitwise comparison in hex: "_ & f" = "_" for any value of "_", and "_ & 0" = "0"
  // This is because 0xf = 15 in decimal = 1111 in binary, while 0x0 = 0 = 0000
  // Thus, "0x____ & 0xf000" = "0x_000"
  if ( (_16bit_word_a & 0xf000) != 0x5000 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if (*_error_status != mtf7::NO_ERROR) return 0;
  
  // Pick the AMC13 block (What does this mean? -AWB 05.12.15)
  emutf_amc13_header_block & _unpacked_block_event_info = *(_unpacked_event_info -> _emutf_amc13_header_block);
  
  // "Word >> X" returns "Word" shifted right by X binary bits (X/4 hex bits)
  // Thus when you read a bit out of "Word", instead of reading bit Y you will read bit Y-X
  // "& 0xff" reads the right-most 8 binary bits (2 hex bits), "& 0xf" the right-most 4, 
  // "& 0x7" the right-most 3, "& 0x3" the right-most 2, and "& 0x1" the right-most 1
  // "<< X" shifts the value assignent to the left by X bits.  
  // Thus, if I say Y = 0x00f << 8, it is equivalent to Y = 0xf00
  // "|=" is the bitwise "OR" assignment. As used here, it is basically addition, 
  // since the things being "OR-ed" do not have contents in the same bits.
  _unpacked_block_event_info . _amc13_header_evt_ty     = (_16bit_word_a >> 8) & 0xf;   // Evt_ty    = bits 8 - 11 of word_a
  _unpacked_block_event_info . _amc13_header_lv1_id     = (_16bit_word_a & 0xff) << 16; // LV1_id    = bits 0 -  7 of word_a
  _unpacked_block_event_info . _amc13_header_lv1_id    |=  _16bit_word_b & 0xffff;      //           + bits 0 - 15 of word_b
  _unpacked_block_event_info . _amc13_header_bx_id      = (_16bit_word_c >> 4) & 0xfff; // BX_id     = bits 4 - 15 of word_c
  _unpacked_block_event_info . _amc13_header_source_id  = (_16bit_word_c & 0xf) << 8;   // Source_id = bits 0 -  3 of word_c
  _unpacked_block_event_info . _amc13_header_source_id |= (_16bit_word_d >> 8) & 0xff;  //           + bits 8 - 15 of word_d
  _unpacked_block_event_info . _amc13_header_fov        = (_16bit_word_d >> 4) & 0xf;   // FOV       = bits 4 -  7 of word_d
  _unpacked_block_event_info . _amc13_header_h          = (_16bit_word_d >> 3) & 0x1;   // H         = bit       3 of word_d
  _unpacked_block_event_info . _amc13_header_x          = (_16bit_word_d >> 2) & 0x1;   // x         = bit       2 of word_d
  // word_d bits 1 and 2 labeled as "$" in docs/UpdatedDAQPath_2015-09-30.pdf
  
  // - - - - - - - - - - - - - - - - - - - - - 
  // Unpack 2nd 64-bit word
  break_into_abcd_words( *at_ptr ); at_ptr++;
  
  // This would permanently shift _16bit_word_a right by 4 bits ... I think the method used above is cleaner.  AWB 05.12.15
  // _16bit_word_a >>=4;
  _unpacked_block_event_info . _amc13_header_ufov = (_16bit_word_a >> 12) & 0xf;    // uFOV = bits 12 - 15 of word_a 
  _unpacked_block_event_info . _amc13_header_res  = (_16bit_word_a >> 8) & 0xf;     // Res  = bits  8 - 11 of word_a
  _unpacked_block_event_info . _amc13_header_namc = (_16bit_word_a >> 4) & 0xf;     // nAMC = bits  4 -  7 of word_a
  // word_a bits 0 - 3 and word_b bits 4 - 15 labeled as "Reserved(0)" in docs/UpdatedDAQPath_2015-09-30.pdf
  _unpacked_block_event_info . _amc13_header_orn  = (_16bit_word_b & 0xf) << 28;    // OrN  = bits  0 -  3 of word_b 
  _unpacked_block_event_info . _amc13_header_orn |= (_16bit_word_c & 0xffff) << 12; //      + bits  0 - 15 of word_c  
  _unpacked_block_event_info . _amc13_header_orn |= (_16bit_word_d >> 4) & 0xfff;   //      + bits  4 - 15 of word_d                             
  
  // Check that bits 0 - 3 of word_d = 0x0
  if ( (_16bit_word_d & 0xf) != 0x0 ) { *_error_status = mtf7::EVENT_RECORD_FORMAT; }
  if (*_error_status != mtf7::NO_ERROR) return 0;
  
  MTF7_DEBUG( std::cout, (int)_unpacked_block_event_info . _amc13_header_evt_ty);
  MTF7_DEBUG( std::cout, (int)_unpacked_block_event_info . _amc13_header_bx_id);
  MTF7_DEBUG( std::cout, (int)_unpacked_block_event_info . _amc13_header_fov);
  MTF7_DEBUG( std::cout, (int)_unpacked_block_event_info . _amc13_header_res);
  MTF7_DEBUG( std::cout, (int)_unpacked_block_event_info . _amc13_header_ufov);
  MTF7_DEBUG( std::cout, (int)_unpacked_block_event_info . _amc13_header_orn);
  
  return at_ptr;
}

unsigned long mtf7::emutf_amc13_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;
  
  // Pick the AMC13 block (What does this mean? -AWB 05.12.15)
  emutf_amc13_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_amc13_header_block;
  
  // - - - - - - - - - - - - - - - - - - - - - 
  // Pack 1st 64-bit word: CDF Header
  _16bit_word_a  =  0x5 << 12;
  _16bit_word_a |= (_block_event_info_to_pack -> _amc13_header_evt_ty & 0xf) << 8;
  _16bit_word_a |= (_block_event_info_to_pack -> _amc13_header_lv1_id >> 16) & 0xff;
  
  _16bit_word_b  =  _block_event_info_to_pack  -> _amc13_header_lv1_id & 0xffff;
  
  _16bit_word_c  = (_block_event_info_to_pack -> _amc13_header_bx_id & 0xfff) << 4;
  _16bit_word_c |= (_block_event_info_to_pack -> _amc13_header_source_id >> 8) & 0xf;
  
  _16bit_word_d  = (_block_event_info_to_pack -> _amc13_header_source_id & 0xff) << 8;
  _16bit_word_d |= (_block_event_info_to_pack -> _amc13_header_fov & 0xf) << 4;
  _16bit_word_d |= (_block_event_info_to_pack -> _amc13_header_h & 0x1) << 3;
  _16bit_word_d |= (_block_event_info_to_pack -> _amc13_header_x & 0x1) << 2;
  // Why don't we fill bits 1 and 2 (labeled "$" in docs/UpdatedDAQPath_2015-09-30.pdf) here? - AWB 05.12.15
  
  *ptr = merge_abcd_words(); ptr++;
  
  // - - - - - - - - - - - - - - - - - - - - - 
  // Pack 2nd 64-bit word
  _16bit_word_a  = (_block_event_info_to_pack -> _amc13_header_ufov & 0xf) << 12;
  _16bit_word_a |= (_block_event_info_to_pack -> _amc13_header_res & 0xf) << 8;
  _16bit_word_a |= (_block_event_info_to_pack -> _amc13_header_namc & 0xf) << 4;
  _16bit_word_a |=  0x0;
  
  _16bit_word_b  =  0x000 << 4;
  _16bit_word_b |= (_block_event_info_to_pack -> _amc13_header_orn >> 28) & 0xf;
  
  _16bit_word_c  = (_block_event_info_to_pack -> _amc13_header_orn >> 12) & 0xffff;
  
  _16bit_word_d  = (_block_event_info_to_pack -> _amc13_header_orn & 0xfff) << 4;
  _16bit_word_d |=  0x0;
  
  *ptr = merge_abcd_words(); ptr++;
  
  return _nominal_buffer_size;
}
