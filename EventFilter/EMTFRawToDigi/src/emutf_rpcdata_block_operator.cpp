#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_rpcdata_block_operator.h"
 
const mtf7::word_64bit *mtf7::emutf_rpcdata_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  MTF7_DEBUG_MSG(std::cout, "######### Unpacking in emutf_rpcdata_block_operator.cpp ######### ");

  // Pick the MTF7 RPC data block
  emutf_rpcdata_block * _unpacked_block_event_info = new emutf_rpcdata_block(); 
  _unpacked_block_event_info -> clear_block();

  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  // - - - - - - - - - - - - - - - - - - - - -
  // Unpack 64-bit word: RPC Data Record
  break_into_dcba_words( *at_ptr ); at_ptr++;

  // Check that bit 15 of word_a, word_b, and word_d = 0x0, and bit 15 of word_c = 0x1
  if ( ((_16bit_word_a >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_b >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_c >> 15) & 0x1) != 0x1 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }
  if ( ((_16bit_word_d >> 15) & 0x1) != 0x0 ) { *_error_status = mtf7::BLOCK_COUNTER_FORMAT; }

  // If the block counter format is incorrect return the original pointer so you can attempt to unpack another pointer.
  if (*_error_status == mtf7::BLOCK_COUNTER_FORMAT) { at_ptr--; return at_ptr; }
  else if (*_error_status != mtf7::NO_ERROR) { return 0; }
 
  _unpacked_block_event_info -> _rpc_prt_delay      = (_16bit_word_a >> 12) & 0x7; // Prt. Delay     = bits 12 - 14 of word_a
  _unpacked_block_event_info -> _rpc_partition_num  = (_16bit_word_a >>  8) & 0xf; // Partition Num  = bits  8 - 11 of word_a
  _unpacked_block_event_info -> _rpc_partition_data =  _16bit_word_a & 0xff;       // Partition data = bits  0 -  7 of word_a

  _unpacked_block_event_info -> _rpc_bcn         = (_16bit_word_b >> 8) & 0x7f;    // BCN         = bits 8 - 14 of word_b
  _unpacked_block_event_info -> _rpc_eod         = (_16bit_word_b >> 7) & 0x1;     // EOD         = bit       7 of word_b
  _unpacked_block_event_info -> _rpc_lb          = (_16bit_word_b >> 5) & 0x3;     // LB          = bits 5 -  6 of word_b
  _unpacked_block_event_info -> _rpc_link_number =  _16bit_word_b & 0x1f;          // Link Number = bits 0 -  4 of word_b

  _unpacked_block_event_info -> _rpc_bc0 = (_16bit_word_c >> 14) & 0x1; // BC0     = bit      14 of word_c
  // Bits 12 - 13 of word_c are unassigned
  _unpacked_block_event_info -> _rpc_bxn =  _16bit_word_c & 0xfff;      // RPC BXN = bits 0 - 11 of word_c
  
  // Bits 3 - 14 of word_d are unassigned
  _unpacked_block_event_info -> _rpc_tbin = _16bit_word_d & 0x7; // TBIN Num = bits 0 - 2 of word_d

  // Now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_rpcdata_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}


unsigned long mtf7::emutf_rpcdata_block_operator::pack(){

  // AWB 10.12.15 - TODO: check the emutf_rpcdata_block_operator packer with the latest format

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_rpcdata_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_rpcdata_block_vector.front();
  
  _16bit_word_a = _block_event_info_to_pack -> _rpc_prt_delay & 0x7; _16bit_word_a <<= 4;
  _16bit_word_a |= _block_event_info_to_pack -> _rpc_partition_num & 0xf; _16bit_word_a <<= 8;
  _16bit_word_a |= _block_event_info_to_pack -> _rpc_partition_data & 0xff; 
  
  _16bit_word_b = _block_event_info_to_pack -> _rpc_bcn & 0x3f; _16bit_word_b <<= 1;
  _16bit_word_b |= _block_event_info_to_pack -> _rpc_eod & 0x1; _16bit_word_b <<= 2;
  _16bit_word_b |= _block_event_info_to_pack -> _rpc_lb & 0x3; _16bit_word_b <<= 5;
  _16bit_word_b |= _block_event_info_to_pack -> _rpc_link_number & 0x1f;

  _16bit_word_c = _block_event_info_to_pack -> _rpc_bc0 & 0x1; _16bit_word_c <<= 14;
  _16bit_word_c |= _block_event_info_to_pack -> _rpc_bxn & 0xff;
  _16bit_word_c |= 0x8000;

  _16bit_word_d = _block_event_info_to_pack -> _rpc_tbin & 0x7;

  
  *ptr = merge_dcba_words();

  return _nominal_buffer_size;


}
