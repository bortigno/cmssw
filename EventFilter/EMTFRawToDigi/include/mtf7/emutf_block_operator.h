#ifndef __MTF7_EMUTF_BLOCK_OPERATOR__
#define __MTF7_EMUTF_BLOCK_OPERATOR__

#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event.h"
#include <iostream>
namespace mtf7{

  class emutf_block_operator: public block_operator{

  public:

    // Constructor with error status only. Minimal constructor required by block operator.
    emutf_block_operator( error_value *error_status ): block_operator (error_status) {} 

    // Constructor with also event info to unpack for higher level operations.
    emutf_block_operator( error_value *error_status, emutf_event *unpacked_event_info ): block_operator(error_status), 
      _unpacked_event_info( unpacked_event_info ) {}

    // Constructor with also event info to pack for higher level operations.
    emutf_block_operator( error_value *error_status, const emutf_event *event_info_to_pack ): block_operator(error_status), 
      _event_info_to_pack( event_info_to_pack ) {}


    virtual unsigned long        pack   ()                         = 0; 
    virtual const word_64bit    *unpack (const word_64bit *at_ptr) = 0; 

    const emutf_event *get_unpacked_event_info(){ return _unpacked_event_info; }
    
    void set_event_info_to_pack     ( const emutf_event *event_info ){ _event_info_to_pack  = event_info; }
    void set_unpacked_event_info_ptr( emutf_event *event_info )      { _unpacked_event_info = event_info; }

  protected:

    emutf_event       *_unpacked_event_info;
    const emutf_event *_event_info_to_pack;
    
  };

}

#endif
