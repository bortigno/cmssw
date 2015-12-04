#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_prepayload_header_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"

const mtf7::word_64bit *mtf7::emutf_prepayload_header_block_operator::unpack ( const mtf7::word_64bit *at_ptr ){
        std::cout << "Unpacking the prepayload header" << std::endl;
        // pick the pre payload header data block
        emutf_prepayload_header_block * _unpacked_block_event_info = new emutf_prepayload_header_block();
        _unpacked_block_event_info -> clear_block();
        std::cout << "Prepayload Header Checkpoint 1" << std::endl;
        if (*_error_status != mtf7::NO_ERROR) return 0;
        std::cout << "Prepayload Header Checkpoint 2" << std::endl;

        if (at_ptr == 0) { *_error_status = mtf7::NULL_BUFFER_PTR; return 0; }
        std::cout << "Prepayload Header Checkpoint 3" << std::endl;

        break_into_abcd_words( *at_ptr ); at_ptr++;
        std::cout << "Prepayload Header Checkpoint 4" << std::endl;
        if ( (_16bit_word_b & 0x8000 ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D15 is 0 
        }else{
                std::cout << "No error at location 1" << std::endl;
        }
        if ( (_16bit_word_b & 0x4000 ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D14 is 0
        }else{
                std::cout << "No error at location 2" << std::endl;
        }
        if ( (_16bit_word_b & 0x2000 ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D13 is 0
        }else{
                std::cout << "No error at location 3" << std::endl;
        }
        if ( (_16bit_word_b & 0x1000 ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D12 is 0
        }else{
                std::cout << "No error at location 4" << std::endl;
        }

        std::cout << "Prepayload Header Checkpoint 5" << std::endl;

        if ( (_16bit_word_d & 0x8000 ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D15 is 0
        }else{
                std::cout << "No error at location 5" << std::endl;
        }
        
        /* THESE BITS ARE NOT ZERO.  THEY ARE AMC_SIZE(24) BITS 16-23
        if ( (_16bit_word_d & 0x1    ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D0  is 0
        }else{
                std::cout << "No error at location 6" << std::endl;
        }
        if ( (_16bit_word_d & 0x2    ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D1  is 0  ERROR HERE
        }else{
                std::cout << "No error at location 7" << std::endl;
        }
        if ( (_16bit_word_d & 0x4    ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D2  is 0  ERROR HERE
        }else{
                std::cout << "No error at location 8" << std::endl;
        }
        if ( (_16bit_word_d & 0x8    ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D3  is 0
        }else{
                std::cout << "No error at location 9" << std::endl;
        }
        if ( (_16bit_word_d & 0x10   ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D4  is 0  ERROR HERE
        }else{
                std::cout << "No error at location 10" << std::endl;
        }
        if ( (_16bit_word_d & 0x20   ) != 0x0000 ){ *_error_status = mtf7::BLOCK_COUNTER_FORMAT; // check if D5  is 0
        }else{
                std::cout << "No error at location 11" << std::endl;
        }
*/
std::cout << "Prepayload Header Checkpoint 6" << std::endl;
  if (*_error_status != mtf7::NO_ERROR) return 0;

std::cout << "Prepayload Header Checkpoint 7" << std::endl;
  _unpacked_block_event_info -> _prepayload_boardid = (_16bit_word_a & 0xffff);

  _unpacked_block_event_info -> _prepayload_amcno = (_16bit_word_b & 0xf);
  _unpacked_block_event_info -> _prepayload_blkNo = (_16bit_word_b >> 8 & 0xff);

  _unpacked_block_event_info -> _prepayload_amcn_size = (_16bit_word_c & 0xffff); //Is this line correct?
  _unpacked_block_event_info -> _prepayload_amcn_size |= (_16bit_word_d & 0xff) << 16;


  _unpacked_block_event_info -> _prepayload_l = (_16bit_word_d >> 7 & 0x1); _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _prepayload_m = (_16bit_word_d >> 0x1); _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _prepayload_s = (_16bit_word_d >> 0x1); _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _prepayload_e = (_16bit_word_d >> 0x1); _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _prepayload_p = (_16bit_word_d >> 0x1); _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _prepayload_v = (_16bit_word_d >> 0x1); _16bit_word_d >>= 1;
  _unpacked_block_event_info -> _prepayload_c = (_16bit_word_d >> 0x1); _16bit_word_d >>= 1;

  // now fill the vector of blocks in the event
  _unpacked_event_info -> _emutf_prepayload_header_block_vector.push_back(_unpacked_block_event_info);

  return at_ptr;
}



unsigned long mtf7::emutf_prepayload_header_block_operator::pack(){

  mtf7::word_64bit *buffer = create_buffer ( _nominal_buffer_size );

  mtf7::word_64bit *ptr = buffer;

  // pick the block event info
  emutf_prepayload_header_block * _block_event_info_to_pack = _event_info_to_pack -> _emutf_prepayload_header_block_vector.front();

_16bit_word_a = _block_event_info_to_pack -> _prepayload_boardid & 0xffff;

_16bit_word_b = _block_event_info_to_pack -> _prepayload_blkNo & 0xff; _16bit_word_b <<= 8;
_16bit_word_b = _block_event_info_to_pack -> _prepayload_amcno & 0xf;

_16bit_word_c |= _block_event_info_to_pack -> _prepayload_amcn_size & 0xffff;
_16bit_word_c |= 0x5000;
_16bit_word_d |= _block_event_info_to_pack -> _prepayload_amcn_size & 0xff;

_16bit_word_d = _block_event_info_to_pack -> _prepayload_c & 0x1; _16bit_word_d <<= 1;
_16bit_word_d = _block_event_info_to_pack -> _prepayload_v & 0x1; _16bit_word_d <<= 1;
_16bit_word_d = _block_event_info_to_pack -> _prepayload_p & 0x1; _16bit_word_d <<= 1;
_16bit_word_d = _block_event_info_to_pack -> _prepayload_e & 0x1; _16bit_word_d <<= 1;
_16bit_word_d = _block_event_info_to_pack -> _prepayload_s & 0x1; _16bit_word_d <<= 1;
_16bit_word_d = _block_event_info_to_pack -> _prepayload_m & 0x1; _16bit_word_d <<= 1;
_16bit_word_d = _block_event_info_to_pack -> _prepayload_l & 0x1; _16bit_word_d <<= 6;


  *ptr = merge_abcd_words();

  return _nominal_buffer_size;
}
