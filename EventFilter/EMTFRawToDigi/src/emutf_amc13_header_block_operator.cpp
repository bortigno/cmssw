#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc13_header_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"
#include <iostream>
//----------------------------------------------------------------------
// mtf7::emutf_header_operator::emutf_amc13_header_block_operator( error_value *error_status, emutf_event *event_info ): emutf_block_operator( error_status, event_info ){}

//----------------------------------------------------------------------
const mtf7::word_64bit *mtf7::emutf_amc13_header_block_operator::unpack( const word_64bit *at_ptr ){
////std::cout << "Unpacking emutf_amc13_header_block_operator" << std::endl;
std::cout << "Begin unpacking AMC13 Header" << std::endl;
   if (*_error_status != mtf7::NO_ERROR) return 0;
std::cout << "Checkpoint 1" << std::endl;
   _buffer_start_ptr = at_ptr;

   if (at_ptr== 0){
	   std::cout << "Checkpoint 1.1" << std::endl;
	   *_error_status = mtf7::NULL_BUFFER_PTR; return 0;
   }
   std::cout << "Checkpoint 2" << std::endl;

   // - - - - - - - - - - - - - - - - - - - - - 
   // process 1st 64-bit word ...
   // CDF header
   break_into_abcd_words( *at_ptr); at_ptr++;
   std::cout << "Checkpoint 3" << std::endl;

   // check format identifiers
   if ( (_16bit_word_a & 0xf000) != 0x5000 ){
std::cout << "16bit word a: " << _16bit_word_a << std::endl;	
std::cout << "16bit word b: " << _16bit_word_b << std::endl;	
std::cout << "16bit word c: " << _16bit_word_c << std::endl;	
std::cout << "16bit word d: " << _16bit_word_d << std::endl;	

   std::cout << "Checkpoint 3.1" << std::endl; 
	   *_error_status = mtf7::EVENT_RECORD_FORMAT;
   }
   std::cout << "Checkpoint 4" << std::endl;
   if (*_error_status != mtf7::NO_ERROR) return 0;
   std::cout << "Checkpoint 5" << std::endl;

  // pick the amc13 block
  emutf_amc13_header_block & _unpacked_block_event_info = *(_unpacked_event_info -> _emutf_amc13_header_block);

  std::cout << "nAMC value pre unpacking: " << (int)_unpacked_event_info -> _emutf_amc13_header_block -> _amc13_header_namc << std::endl;

  std::cout << "Checkpoint 5.1" << std::endl;

  _unpacked_block_event_info . _amc13_header_evt_ty = (_16bit_word_a >> 8) & 0xf;
  _unpacked_block_event_info . _amc13_header_lv1_id = _16bit_word_b & 0xffff;
  _unpacked_block_event_info . _amc13_header_lv1_id |= (_16bit_word_a & 0xff) << 16;
  _unpacked_block_event_info . _amc13_header_bx_id = (_16bit_word_c >> 4) & 0xfff;
  _unpacked_block_event_info . _amc13_header_source_id = (_16bit_word_c & 0xf) << 8;
  _unpacked_block_event_info . _amc13_header_source_id |= (_16bit_word_d >> 8) & 0xff;
  _unpacked_block_event_info . _amc13_header_x = (_16bit_word_d >> 2) & 0x1;
  _unpacked_block_event_info . _amc13_header_h = (_16bit_word_d >> 3) & 0x1;
  _unpacked_block_event_info . _amc13_header_fov = (_16bit_word_d >> 4) & 0xf;

  // - - - - - - - - - - - - - - - - - - - - - 
  // 2nd 64-bit word ...
   std::cout << "Checkpoint 6" << std::endl;
  break_into_abcd_words( *at_ptr); at_ptr++;
   std::cout << "Checkpoint 7" << std::endl;

  // set values derived from 1st word
   std::cout << "namc in words = " << (_16bit_word_a & 0xf) << std::endl ;
  _unpacked_block_event_info . _amc13_header_namc  =  _16bit_word_a & 0xf; _16bit_word_a <<= 4;
  _unpacked_block_event_info . _amc13_header_res   =  _16bit_word_a & 0xf; _16bit_word_a <<= 4;
  _unpacked_block_event_info . _amc13_header_ufov  =  _16bit_word_a & 0xf;



  std::cout << "nAMC value = " << (int)_unpacked_block_event_info . _amc13_header_namc << std::endl;
  std::cout << "l1 id value = " << _unpacked_block_event_info . _amc13_header_lv1_id << std::endl;
  std::cout << "Source id = " << _unpacked_block_event_info . _amc13_header_source_id << std::endl;

  // next 16 bits are reserved and set to 0.

  _unpacked_block_event_info . _amc13_header_orn = _16bit_word_b & 0xfff0;
  _unpacked_block_event_info . _amc13_header_orn |= (_16bit_word_c & 0xffff) << 12;
  //_unpacked_block_event_info . _amc13_header_orn |= (_16bit_word_a & 0xf) << 28;
  _unpacked_block_event_info . _amc13_header_orn |= (_16bit_word_d & 0xf) << 28;


  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _amc13_header_evt_ty);
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _amc13_header_bx_id);
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _amc13_header_fov);
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _amc13_header_res);
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _amc13_header_ufov);
  MTF7_DEBUG( std::cout, _unpacked_block_event_info . _amc13_header_orn);

   std::cout << "Checkpoint 8" << std::endl;
  return at_ptr;

}

unsigned long mtf7::emutf_amc13_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_amc13_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_amc13_header_block;

  // pack 1st 64bit word --------------------
  // CDF header

  _16bit_word_a = _block_event_info_to_pack ->_amc13_header_evt_ty & 0xf; _16bit_word_a <<= 8;
  _16bit_word_a |= (_block_event_info_to_pack -> _amc13_header_lv1_id >> 16) & 0xff;
  _16bit_word_a |= 0x5000;

  _16bit_word_b = _block_event_info_to_pack -> _amc13_header_lv1_id & 0xffff;

  _16bit_word_c = _block_event_info_to_pack -> _amc13_header_bx_id & 0xfff; _16bit_word_c <<= 4;
  _16bit_word_c |= (_block_event_info_to_pack -> _amc13_header_source_id >> 8) & 0xf;

  _16bit_word_d = _block_event_info_to_pack -> _amc13_header_source_id & 0xff; _16bit_word_d <<= 4;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_header_fov & 0xf; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_header_h & 0x1; _16bit_word_d <<= 1;
  _16bit_word_d |= _block_event_info_to_pack -> _amc13_header_x & 0x1; _16bit_word_d <<= 2;


  *ptr = merge_abcd_words(); ptr++;

  // pack 2nd 64bit word --------------------

  _16bit_word_a = _block_event_info_to_pack -> _amc13_header_ufov & 0xf; _16bit_word_a <<= 4;
  _16bit_word_a |= _block_event_info_to_pack -> _amc13_header_res & 0xf; _16bit_word_a <<= 4;
  _16bit_word_a |= _block_event_info_to_pack -> _amc13_header_namc & 0xf; _16bit_word_a <<= 4;
  _16bit_word_a |= 0x0;

  _16bit_word_b = 0x0000;
  _16bit_word_b |= (_block_event_info_to_pack -> _amc13_header_orn >> 28) & 0xf;

  _16bit_word_c = (_block_event_info_to_pack -> _amc13_header_orn >> 12) & 0xffff;
  _16bit_word_d = _block_event_info_to_pack -> _amc13_header_orn & 0xfff; _16bit_word_d <<= 4;
  _16bit_word_d |= 0x0;


  *ptr = merge_abcd_words(); ptr++;
  

  return _nominal_buffer_size;

}
