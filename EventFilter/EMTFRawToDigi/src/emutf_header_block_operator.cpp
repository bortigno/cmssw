#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_header_block_operator.h"

//----------------------------------------------------------------------
const mtf7::word_64bit *mtf7::emutf_header_block_operator::unpack( const word_64bit *at_ptr ){

// pick the header block
emutf_header_block * _unpacked_block_event_info = new emutf_header_block(); 
_unpacked_block_event_info -> clear_block();

std::cout << "Begin unpacking EMUTF header block operator" << std::endl;
std::cout << "Checkpoint 1" << std::endl;
   if (*_error_status != mtf7::NO_ERROR) return 0;
std::cout << "Checkpoint 2" << std::endl;

   _buffer_start_ptr = at_ptr;

  if (at_ptr== 0){ *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }
std::cout << "Checkpoint 3" << std::endl;

  // - - - - - - - - - - - - - - - - - - - - - 
  // process 1st 64-bit word ...
  break_into_abcd_words( *at_ptr); at_ptr++;
std::cout << "Checkpoint 4" << std::endl;
std::cout << "Print abcd words for first 64 bit word" << std::endl;
std::cout << "Word a: " << _16bit_word_a << std::endl;
std::cout << "Word b: " << _16bit_word_b << std::endl;
std::cout << "Word c: " << _16bit_word_c << std::endl;
std::cout << "Word d: " << _16bit_word_d << std::endl;
  // check format identifiers
if ( (_16bit_word_a & 0xf000) != 0x9000 ){ 
	*_error_status = mtf7::EVENT_RECORD_FORMAT;
	std::cout << "Checkpoint 4.0.1" << std::endl;
}
std::cout << "Checkpoint 4.1" << std::endl;
if ( (_16bit_word_b & 0xf000) != 0x9000 ) {
*_error_status = mtf7::EVENT_RECORD_FORMAT;
std::cout << "Checkpoint 4.1.1" << std::endl;
}
std::cout << "Checkpoint 4.2" << std::endl;
if (  _16bit_word_c           != 0x9000 ){
 *_error_status = mtf7::EVENT_RECORD_FORMAT;
std::cout << "Checkpoint 4.2.1" << std::endl;
}
std::cout << "Checkpoint 4.3" << std::endl;
if ( _16bit_word_c & 0xfff != 0x000 ){
 *_error_status = mtf7::EVENT_RECORD_FORMAT; //BAM
std::cout << "Checkpoint 4.3.1" << std::endl;
}
std::cout << "Checkpoint 4.4" << std::endl;
if ( (_16bit_word_d & 0xf000) != 0x9000 ){
 *_error_status = mtf7::EVENT_RECORD_FORMAT;
std::cout << "Checkpoint 4.4.1" << std::endl;
}
std::cout << "Checkpoint 4.5" << std::endl;
if (*_error_status != mtf7::NO_ERROR) return 0;
std::cout << "Checkpoint 4.6" << std::endl;

  // set values derived from 1st word
  _unpacked_block_event_info -> _l1a  =  _16bit_word_a & 0xfff;
std::cout << "Checkpoint 4.7" << std::endl;
  _unpacked_block_event_info -> _l1a |= (_16bit_word_b & 0xfff) << 12;
std::cout << "Checkpoint 4.8" << std::endl;
  _unpacked_block_event_info -> _bxn  =  _16bit_word_d & 0xfff;
std::cout << "Checkpoint 4.9" << std::endl;

  // - - - - - - - - - - - - - - - - - - - - - 
  // 2nd 64-bit word ...
std::cout << "Checkpoint 5" << std::endl;
  break_into_abcd_words( *at_ptr); at_ptr++;

    // check format identifiers
  if (  _16bit_word_a           != 0xa000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( _16bit_word_a & 0xfff != 0x000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT; //BAM
  if ( (_16bit_word_b & 0xf000) != 0xa000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_c & 0xf000) != 0xa000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_c & 0x800 )           ) *_error_status = mtf7::EVENT_RECORD_FORMAT;
  if ( (_16bit_word_d & 0xf000) != 0xa000 ) *_error_status = mtf7::EVENT_RECORD_FORMAT;

  if (*_error_status != mtf7::NO_ERROR) return 0;
std::cout << "Checkpoint 6" << std::endl;

  _unpacked_block_event_info -> _sp_addr =  _16bit_word_b & 0x1f;
  _unpacked_block_event_info -> _sp_ersv = (_16bit_word_b >> 5) & 0x7;
  _unpacked_block_event_info -> _sp_ts   = (_16bit_word_b >> 8) & 0xf;

  _unpacked_block_event_info -> _wof     = (_16bit_word_c & 0x1); _16bit_word_c >>=1;
  _unpacked_block_event_info -> _osy     = (_16bit_word_c & 0x1); _16bit_word_c >>=1;
  _unpacked_block_event_info -> _bsy     = (_16bit_word_c & 0x1); _16bit_word_c >>=1;
  _unpacked_block_event_info -> _rdy     = (_16bit_word_c & 0x1); _16bit_word_c >>=1;
  _unpacked_block_event_info -> _skip    = (_16bit_word_c & 0x1); _16bit_word_c >>=1;
  _unpacked_block_event_info -> _rpca    = (_16bit_word_c & 0x1); _16bit_word_c >>=1;
  _unpacked_block_event_info -> _spa     = (_16bit_word_c & 0x1); _16bit_word_c >>=1;
  _unpacked_block_event_info -> _ddm     = (_16bit_word_c & 0x1); _16bit_word_c >>=1;

  _unpacked_block_event_info -> _tbin    = _16bit_word_c & 0x7; // IKF todo: size of at_ptr from _tbin and _skip
  
  _unpacked_block_event_info -> _ME1a    = _16bit_word_d & 0x1ff;

  // - - - - - - - - - - - - - - - - - - - - - 
  // third 64-bit word ...
  break_into_abcd_words( *at_ptr); at_ptr++;

std::cout << "Checkpoint 7" << std::endl;
  // check format identifiers
  if ( (_16bit_word_a & 0x8000) != 0x8000)            *_error_status = mtf7::EVENT_RECORD_FORMAT;
std::cout << "Checkpoint 8" << std::endl;
  if ( (_16bit_word_b & 0x8000) != 0x0000)             *_error_status = mtf7::EVENT_RECORD_FORMAT;
std::cout << "Checkpoint 9" << std::endl;
  if ( (_16bit_word_c & 0x8000) != 0x0000)             *_error_status = mtf7::EVENT_RECORD_FORMAT;
std::cout << "Checkpoint 10" << std::endl;
  if ( (_16bit_word_d & 0x8000) != 0x0000)             *_error_status = mtf7::EVENT_RECORD_FORMAT;
std::cout << "Checkpoint 11" << std::endl;

  if (*_error_status != mtf7::NO_ERROR) return 0;
std::cout << "Checkpoint 12" << std::endl;

  _unpacked_block_event_info -> _ME1b =  _16bit_word_a & 0x1ff; _16bit_word_a >>= 9;
  _unpacked_block_event_info -> _RPC  =  _16bit_word_a & 0x3f;

  _unpacked_block_event_info -> _ME2  =  _16bit_word_b & 0x1ff; _16bit_word_b >>= 9;
  _unpacked_block_event_info -> _RPC |= (_16bit_word_b & 0x3f) << 6;

  _unpacked_block_event_info -> _ME3  =  _16bit_word_c & 0x1ff; _16bit_word_c >>= 9;
  _unpacked_block_event_info -> _RPC |= (_16bit_word_c & 0x3f) << 12;
 
  _unpacked_block_event_info -> _ME4  =  _16bit_word_d & 0x1ff; _16bit_word_d >>= 9;
  _unpacked_block_event_info -> _RPC |= (_16bit_word_d & 0x3f) << 18;


  // now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_header_block_vector.push_back(_unpacked_block_event_info);

  std::cout << "Checkpoint 13" << std::endl;
  return at_ptr;

}

unsigned long mtf7::emutf_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_header_block_vector.front();

  // pack 1st 64bit word --------------------

  mtf7::word_32bit tmp_l1a = _block_event_info_to_pack -> _l1a;

  _16bit_word_a = 0x9000 | (tmp_l1a & 0xfff); tmp_l1a >>= 12;
  _16bit_word_b = 0x9000 | (tmp_l1a & 0xfff);
  _16bit_word_c = 0x9000;
  _16bit_word_d = 0x9000 | (_block_event_info_to_pack -> _bxn & 0xfff);

  *ptr = merge_abcd_words(); ptr++;

  // pack 2nd 64bit word --------------------

  _16bit_word_a = 0xa000;
  _16bit_word_b = _block_event_info_to_pack -> _sp_ts & 0xf; _16bit_word_b <<= 3;
  _16bit_word_b |= _block_event_info_to_pack -> _sp_ersv & 0x7; _16bit_word_b <<= 5;
  _16bit_word_b |= _block_event_info_to_pack -> _sp_addr & 0x1f;
  _16bit_word_b |= 0xa000;

  _16bit_word_c = _block_event_info_to_pack -> _tbin & 0x7; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _ddm & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _spa & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _rpca & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _skip & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _rdy & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _bsy & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _osy & 0x1; _16bit_word_c <<= 1;
  _16bit_word_c |= _block_event_info_to_pack -> _wof & 0x1; 
  _16bit_word_c |= 0xa000;
  
  _16bit_word_d = _block_event_info_to_pack -> _ME1a & 0x7ff;
  _16bit_word_d |= 0xa000;

  *ptr = merge_abcd_words(); ptr++;

  // pack 3rd 64bit word --------------------

  mtf7::word_32bit tmp_rpc = _block_event_info_to_pack -> _RPC;

  _16bit_word_a = tmp_rpc & 0x3f; _16bit_word_a <<= 9;
  _16bit_word_a |= _block_event_info_to_pack -> _ME1b & 0x7ff;
  _16bit_word_a |= 0x8000;
  tmp_rpc >>= 6;
  _16bit_word_b = tmp_rpc & 0x3f; _16bit_word_b <<= 9;
  _16bit_word_b |= _block_event_info_to_pack -> _ME2 & 0x7ff;
  tmp_rpc >>= 6;
  _16bit_word_c = tmp_rpc & 0x3f; _16bit_word_c <<= 9;
  _16bit_word_c |= _block_event_info_to_pack -> _ME3 & 0x7ff;
  tmp_rpc >>= 6;
  _16bit_word_d = tmp_rpc & 0x3f; _16bit_word_d <<= 9;
  _16bit_word_d |= _block_event_info_to_pack -> _ME4 & 0x7ff;


  *ptr = merge_abcd_words(); ptr++;

  return _nominal_buffer_size;

}
