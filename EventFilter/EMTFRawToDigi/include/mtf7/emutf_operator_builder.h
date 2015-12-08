#ifndef __MTF7_EMUTF_OPERATOR_BUILDER__
#define __MTF7_EMUTF_OPERATOR_BUILDER__

#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/data_operator_builder.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_data_operator.h"

namespace mtf7{


  class emutf_operator_builder {

  public:

    emutf_operator_builder(){}

    emutf_data_operator *get_data_operator( const char *data_release );

  protected:

    block_operator_vector *assemble_block_vector( const char *data_release, error_value *error_ptr );


  };

}

#endif

