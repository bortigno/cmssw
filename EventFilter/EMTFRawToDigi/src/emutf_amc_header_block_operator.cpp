#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc_header_block_operator.h"

const mtf7::word_64bit *mtf7::emutf_amc_header_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  // pick the csc me data block
  emutf_amc_header_block * _unpacked_block_event_info = new emutf_amc_header_block(); 
  _unpacked_block_event_info -> clear_block();

//std::cout << "Unpacking emutf_amc_header_block_operator" << std::endl;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  break_into_abcd_words( *at_ptr ); at_ptr++;
  if ( (_16bit_word_a & 0xf000 ) != 0x0000 ) *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D15 is 0
  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info -> _amc_header_amc_number = (_16bit_word_a & 0x0f00 );
  _unpacked_block_event_info -> _amc_header_lv1_id = (( _16bit_word_a & 0x00ff ) << 16 ) || (_16bit_word_b & 0xffff);


  _unpacked_block_event_info -> _amc_header_bx_id = (_16bit_word_c & 0xfff0);

  _unpacked_block_event_info -> _amc_header_data_length = ((_16bit_word_c & 0x000f) << 16) || (_16bit_word_d & 0xfff);


  // 2nd 64 bit word
  break_into_abcd_words( *at_ptr ); at_ptr++;

  _unpacked_block_event_info -> _amc_header_user_id = ((_16bit_word_a & 0xffff) << 16) || (_16bit_word_b & 0xffff);
  _unpacked_block_event_info -> _amc_header_orbit_number = (_16bit_word_c & 0xffff);
  _unpacked_block_event_info -> _amc_header_board_id = (_16bit_word_d & 0xffff);


  // now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_amc_header_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}


unsigned long mtf7::emutf_amc_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_amc_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_amc_header_block_vector.front();


  _16bit_word_a = _block_event_info_to_pack -> _amc_header_amc_number & 0xf;

  // TO DO : TO COMPLETE!!

  *ptr = merge_abcd_words(); ptr++;

  //here it goes the second word

  _16bit_word_d = _block_event_info_to_pack -> _amc_header_amc_number & 0xffff;

  // TO DO : TO COMPLETE!!

  *ptr = merge_abcd_words();

  return _nominal_buffer_size;


}
