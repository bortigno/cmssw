#ifndef __MTF7_BLOCK_OPERATOR__
#define __MTF7_BLOCK_OPERATOR__

#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"
#include <vector>
#include <iostream>
#include <iomanip>

namespace mtf7{

  class block_operator{

  public:
    
    block_operator( error_value *error_status );
    
    virtual unsigned long        pack   ()                         = 0; 
    virtual const word_64bit    *unpack (const word_64bit *at_ptr) = 0; 
    
    const word_64bit *get_buffer_start_ptr();
    
    void free_own_buffer();

    error_value get_error_status();
    
    virtual void  set_event_info_to_pack( const emutf_event * _event_info ) = 0;
    virtual void  set_unpacked_event_info_ptr( emutf_event *event_info )  = 0;

    // ikf todo: add get_event_info() to base class?
//    virtual const emutf_event *get_unpacked_event_info() = 0;

  protected:

    word_64bit *create_buffer( unsigned long buffer_size );

    const word_64bit *_buffer_start_ptr;
    mtf7_bool  _i_own_buffer;
    error_value *_error_status;
    bool is_bit_true ( mtf7::word_16bit word, int bit_num);

    void break_into_abcd_words ( word_64bit input_value );
    word_64bit merge_abcd_words();
    void break_into_dcba_words ( word_64bit input_value );
    word_64bit merge_dcba_words();

    word_16bit _16bit_word_a, _16bit_word_b, _16bit_word_c, _16bit_word_d;
    
  };

  typedef std::vector<block_operator*>           block_operator_vector;
  typedef block_operator_vector::iterator       block_operator_iterator;
  typedef block_operator_vector::const_iterator block_operator_const_iterator;
  

}

#endif
