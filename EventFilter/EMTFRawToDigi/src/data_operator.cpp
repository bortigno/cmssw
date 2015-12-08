#include <stdlib.h>
#include <cstring>
#include "EventFilter/EMTFRawToDigi/include/mtf7/data_operator.h"

//----------------------------------------------------------------------
mtf7::data_operator::data_operator( const char *dataformat_release ):
  _error_status(), _dataformat_release(0), _workers(0) {

  if (!dataformat_release) return;

  unsigned long string_length = strlen(dataformat_release);

  _dataformat_release = (char *) calloc(string_length+1,1); 

  memcpy ( _dataformat_release, dataformat_release, string_length );

}

//----------------------------------------------------------------------
mtf7::data_operator::~data_operator(){

  if (_dataformat_release)
    free(_dataformat_release);

  _dataformat_release = 0;

  if (_workers)
    delete _workers;

  _workers = 0;

}

//----------------------------------------------------------------------
mtf7::mtf7_bool mtf7::data_operator::is_compatible_with ( const char *dataformat_release )
{  return (strcmp( dataformat_release, _dataformat_release ) == 0); }

