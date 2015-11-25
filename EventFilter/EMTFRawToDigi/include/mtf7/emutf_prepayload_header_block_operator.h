#ifndef __MTF7_EMUTF_PREPAYLOAD_HEADER_BLOCK_OPERATOR__ 
#define __MTF7_EMUTF_PREPAYLOAD_HEADER_BLOCK_OPERATOR__

#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_block_operator.h"

namespace mtf7{

  class emutf_prepayload_header_block_operator: public emutf_block_operator{

  public:

    emutf_prepayload_header_block_operator ( error_value *error_status ):
        emutf_block_operator( error_status ) {}

    emutf_prepayload_header_block_operator ( error_value *error_status, emutf_event *unpacked_event_info ):
        emutf_block_operator( error_status, unpacked_event_info ) {}
    
    emutf_prepayload_header_block_operator ( error_value *error_status, const emutf_event *_event_info_to_unpack ):
        emutf_block_operator( error_status, _event_info_to_unpack ) {}

    virtual unsigned long        pack   ( );
    virtual const word_64bit    *unpack (const word_64bit *at_ptr);

    static const unsigned int _nominal_buffer_size = 1; // buffer size in 64-bit words

  };

}


#endif