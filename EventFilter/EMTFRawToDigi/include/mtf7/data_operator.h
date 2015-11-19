#ifndef __MTF7_DATA_OPERATOR__
#define __MTF7_DATA_OPERATOR__

#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/block_operator.h"

namespace mtf7{

  class data_operator{

  public:

    data_operator( const char *dataformat_release );
    ~data_operator();

    // virtual error_value unpack( const word_64bit *buffer ) = 0;
    // virtual const word_64bit *pack()                       = 0;

    mtf7_bool is_compatible_with ( const char *dataformat_release );

    const char* get_dataformat_release() { return _dataformat_release; }
    error_value get_error_status      () { return _error_status; }

  protected:

    friend class emutf_operator_builder;

    error_value            _error_status;
    char                  *_dataformat_release;
    block_operator_vector *_workers;

  };

}

#endif
