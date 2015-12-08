#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_amc_trailer_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"

const mtf7::word_64bit *mtf7::emutf_amc_trailer_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){

  // pick the csc me data block
  emutf_amc_trailer_block * _unpacked_block_event_info = new emutf_amc_trailer_block(); 
  _unpacked_block_event_info -> clear_block();

//std::cout << "Unpacking emutf_amc_trailer_block_operator" << std::endl;
  if (*_error_status != mtf7::NO_ERROR) return 0;

  if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }

  break_into_abcd_words( *at_ptr ); at_ptr++;
  if ( (_16bit_word_c & 0x00f0 ) != 0x0000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT; 
  if (*_error_status != mtf7::NO_ERROR) return 0;

  _unpacked_block_event_info -> _amc_trailer_crc32 = ((_16bit_word_a & 0x0f00 ) << 16) || (_16bit_word_b & 0xffff);
  _unpacked_block_event_info -> _amc_trailer_lv1_id = (_16bit_word_c & 0xff00);
  _unpacked_block_event_info -> _amc_trailer_data_length = ((_16bit_word_c & 0x000f) << 16) || (_16bit_word_d & 0xffff);
  
  MTF7_DEBUG(std::cout,  _unpacked_block_event_info -> _amc_trailer_lv1_id); 

 // now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_amc_trailer_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;

}


unsigned long mtf7::emutf_amc_trailer_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );
  
  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_amc_trailer_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_amc_trailer_block_vector.front();
  
  _16bit_word_d = _block_event_info_to_pack -> _amc_trailer_crc32 & 0xffff;

  // TO DO : TO COMPLETE!!!

  *ptr = merge_abcd_words();

  return _nominal_buffer_size;


}
