#ifndef __MTF7_types__ 
#define __MTF7_types__ 1

namespace mtf7{

  typedef unsigned short word_16bit;
  typedef unsigned long  word_64bit;
  typedef unsigned int   word_32bit;
  typedef unsigned char  word_8bit;
  typedef bool           word_1bit;
  
  typedef bool           mtf7_bool;
  
  enum error_value {
    NO_ERROR,
    NULL_BUFFER_PTR,
    EVENT_RECORD_FORMAT,
    BLOCK_COUNTER_FORMAT,
    STATUS_UNDEFINED,
    PACK_NULL_EVTINFO,
    PROCESS_INTERNAL
  };
   
}

#endif
