#ifndef __MTF7_DATA_OPERATOR_BUILDER__
#define __MTF7_DATA_OPERATOR_BUILDER__

#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/data_operator.h"

namespace mtf7{


  class data_operator_builder{

  public:

    data_operator_builder(){}

    virtual data_operator *get_data_operator( const char *data_release ) = 0;

  protected:

    virtual block_operator_vector *assemble_block_vector( const char *data_release, error_value *error_ptr ) = 0;

    virtual void initialize_block_operators() {}

  };

}

#endif

