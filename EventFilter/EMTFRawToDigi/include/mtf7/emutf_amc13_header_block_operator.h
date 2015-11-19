#ifndef __MTF7_EMUTF_AMC13_HEADER_OPERATOR__
#define __MTF7_EMUTF_AMC13_HEADER_OPERATOR__


#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_block_operator.h"

namespace mtf7{

  class emutf_amc13_header_block_operator: public emutf_block_operator{

  public:

  	// Minimal constructor with only errro value.
    emutf_amc13_header_block_operator ( error_value *error_status ): 
		emutf_block_operator( error_status ) {}

  	// Constructor with event info to unpack for higher level operations.
    emutf_amc13_header_block_operator ( error_value *error_status, emutf_event *unpacked_event_info ): 
		emutf_block_operator( error_status, unpacked_event_info ) {}

 	// Constructor with event info to pack for higher level operations.
    emutf_amc13_header_block_operator ( error_value *error_status, const emutf_event *_event_info_to_pack ): 
		emutf_block_operator( error_status, _event_info_to_pack ) {}

    virtual unsigned long        pack   ( );
    virtual const word_64bit    *unpack (const word_64bit *at_ptr);

    static const unsigned int _nominal_buffer_size = 2; // buffer size in 64-bit words
  

  };

}

#endif
