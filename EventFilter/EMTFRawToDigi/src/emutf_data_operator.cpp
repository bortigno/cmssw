#include <stdlib.h>
#include <cstring>
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_data_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_debug.h"
#include <iostream>
#include <typeinfo>

//----------------------------------------------------------------------
mtf7::emutf_data_operator::emutf_data_operator( const char *data_release ):
	data_operator(data_release) 
{  }

//----------------------------------------------------------------------
mtf7::error_value mtf7::emutf_data_operator::unpack( const word_64bit *buffer ){
  MTF7_DEBUG_MSG(std::cout, "######### Begin unpacking (main loop): emutf_data_operator.cpp ######### ");
  const word_64bit *tmp_ptr = buffer;

  // ********************************************************
  // AMC13 header: src/emutf_amc13_header_block_operator.cpp
  // ********************************************************
  // _worker 0 in src/emutf_operator_builder.cpp is amc13_header
  emutf_block_operator * _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->front());

  MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for emutf_amc13_header_block_operator");
  _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
  // MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );

  MTF7_DEBUG_MSG( std::cout, "Unpacking block number 0");
  tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

  // Check if pointer is not null, otherwise print the error and the block number
  if(tmp_ptr == 0 ){
    std::cout << "ERR: Error unpacking block number 0" << std::endl;
    std::cout << "ERR: Error value :" << _error_status << std::endl;
    return _error_status; }
  MTF7_DEBUG_MSG(std::cout, "Unpacked block number 0"); MTF7_DEBUG(std::cout, tmp_ptr);
  
  // Number of MTF7 boards sending data to the AMC13. If all boards are connected to the crate this number should always be 6.
  unsigned int nAMC = (unsigned int)_unpacked_event_info -> _emutf_amc13_header_block -> _amc13_header_namc;
  MTF7_DEBUG_MSG(std::cout, "Number of AMC sending data to the AMC13 : " << nAMC); 
  
  // *******************************************************************************************
  // Loop over pre-payload headers (1 per MTF7): src/emutf_prepayload_header_block_operator.cpp
  // *******************************************************************************************
  for ( unsigned int _sp = 0; _sp < nAMC ; _sp++ ) {
    
    // _worker 1 in src/emutf_operator_builder.cpp is prepayload_header
    _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at(1));

    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for emutf_prepayload_header_block_operator");
    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
    // MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );

    MTF7_DEBUG_MSG( std::cout, "Unpacking block number 1");
    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);
    
    // Check if pointer is not null, otherwise print the error and the block number
    if(tmp_ptr == 0 ){
      std::cout << "ERR: Error unpacking block number 1" << std::endl;
      std::cout << "ERR: Error value :" << _error_status << std::endl;
      return _error_status; }
    MTF7_DEBUG_MSG(std::cout, "Unpacked block number 1"); MTF7_DEBUG(std::cout, tmp_ptr);
  }
    
  // All preheaders give information about the size of the payload. If size if 3 or less there is no payload (zero suppression).
  // If size is > 3 then there are tracks to unpack from that specific MTF7 board.
  for( unsigned int _sp = 0; _sp < (unsigned int)_unpacked_event_info -> _emutf_prepayload_header_block_vector.size() ; _sp++ )
    std::cout << "@INFO: Unpacked event info -> _prepayload_amcn_size[" << _sp << "] = " << (unsigned int) _unpacked_event_info -> _emutf_prepayload_header_block_vector.at(_sp) -> _prepayload_amcn_size << std::endl;

  // ********************************
  // Loop over payloads (1 per MTF7)
  // ********************************
  for ( unsigned int _sp = 0; _sp < nAMC ; _sp++ ){

    // _worker 2 in src/emutf_operator_builder.cpp is amc_header
    _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at(2));
    
    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for emutf_amc_header_block_operator");
    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
    // MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
    
    MTF7_DEBUG_MSG( std::cout, "Unpacking block number 2");
    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);
    
    // Check if pointer is not null, otherwise print the error and the block number
    if(tmp_ptr == 0 ){
      std::cout << "ERR: Error unpacking block number 2" << std::endl;
      std::cout << "ERR: Error value :" << _error_status << std::endl;
      return _error_status; }
    MTF7_DEBUG_MSG(std::cout, "Unpacked block number 2 "); MTF7_DEBUG(std::cout, tmp_ptr);
    
    // Check the size of the MTF7 payload.  If it is 3 go directly to the event trailer.
    if ( ((unsigned int) _unpacked_event_info -> _emutf_prepayload_header_block_vector.at(_sp) -> _prepayload_amcn_size) > 3 ) {
      for (block_operator_iterator iter = _workers -> begin()+3; iter != _workers -> begin()+9; iter++){ 
	
	_error_status = mtf7::NO_ERROR;
	// Only for the payload this error check is switched off because it allows to move forward from one block to the other?
	// if (_error_status != NO_ERROR) return _error_status; 
	
	_tmp_block_operator = dynamic_cast<emutf_block_operator *> (*iter);
	MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for payload: _worker " << std::distance(_workers->begin(),iter) );
	_tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
 
	// While you don't get an error (should be BLOCK_COUNTER_ERROR) keep unpacking the same block
	while (_error_status == NO_ERROR ){ 
	  // MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
	  
	  // MTF7_DEBUG_MSG( std::cout, "Unpacking block number "); MTF7_DEBUG(std::cout, std::distance(_workers->begin(),iter));
	  MTF7_DEBUG_MSG( std::cout, "Unpacking block number " << std::distance(_workers->begin(),iter) ); 
	  
	  // Returns 0 if there is an error or the value of the initial pointer if there is a counter format error. (What does this mean? - AWB 12.12.15)
	  tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr); 

	  // Check if pointer is not null, otherwise print the error and the block number
	  if( tmp_ptr == 0 ){
	    std::cout << "ERR: Error value :" << _error_status << std::endl;
	    return _error_status; } 
	  else if ( _error_status != NO_ERROR ){
	   MTF7_DEBUG_MSG( std::cout, "WARN: Unpacking failed with error value :" << _error_status );
	   MTF7_DEBUG_MSG( std::cout, "WARN: Trying to unpack using next block in line." ); } 
	  else{
	    MTF7_DEBUG_MSG(std::cout, "Unpacked block number " << std::distance(_workers->begin(),iter) ); 
	    MTF7_DEBUG(std::cout, tmp_ptr);
	  }	        
	} // End of "while (_error_status == NO_ERROR )" condition  
      } // End of loop over payload workers: "for (block_operator_iterator iter = _workers->at(3); iter != _workers->at(9); iter++)"
    } // End of "if MTF7 has payload" condition: 
      // "if ( ((unsigned int) _unpacked_event_info -> _emutf_prepayload_header_block_vector.at(_sp) -> _prepayload_amcn_size) > 3 )"
    
    _error_status = mtf7::NO_ERROR;
    // Unpack event trailer
    // _worker 9 in src/emutf_operator_builder.cpp is amc_trailer
    _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at(9)); 
    
    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for emutf_amc_trailer_block_operator");
    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
    // MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );

    MTF7_DEBUG_MSG( std::cout, "Unpacking block number 9");
    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);
    
    // Check if pointer is not null, otherwise print the error and the block number
    if(tmp_ptr == 0 ){
      std::cout << "ERR: Error unpacking block number 9" << std::endl;
      std::cout << "ERR: Error value :" << _error_status << std::endl;
      return _error_status; }
    
    MTF7_DEBUG_MSG(std::cout, "Unpacked block number 9"); MTF7_DEBUG(std::cout, tmp_ptr);
  } // End loop over MTF7 payloads: "for ( unsigned int _sp = 0; _sp < nAMC ; _sp++ )"	

 MTF7_DEBUG_MSG( std::cout, "Worker size : " << _workers->size() );
  if (_workers->size() != 12) std::cout << "AWB MAJOR ERROR! Why are there " << _workers->size() << "_workers, not 12!?!" << std::endl;

  // _worker 10 in src/emutf_operator_builder.cpp is postpayload_trailer
  // _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at( _workers->size()-2 ) );
  _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at(10));

  MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for emutf_postpayload_trailer_block_operator");
  _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
  // MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
  
  MTF7_DEBUG_MSG( std::cout, "Unpacking block number 10");
  tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

  // Check if pointer is not null, otherwise print the error and the block number
  if(tmp_ptr == 0 ){
      std::cout << "ERR: Error unpacking block number 10" << std::endl;
      std::cout << "ERR: Error value :" << _error_status << std::endl;
      return _error_status; }
  MTF7_DEBUG_MSG(std::cout, "Unpacked block number 10 "); MTF7_DEBUG(std::cout, tmp_ptr);


  // **********************************************************
  // AMC13 trailer: src/emutf_amc13_trailer_block_operator.cpp
  // **********************************************************
  // _worker 11 in src/emutf_operator_builder.cpp is amc13_trailer
  // _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->back());
  _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at(11));
  
  MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for emutf_amc13_trailer_block_operator");
  _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
  // MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
  
  MTF7_DEBUG_MSG( std::cout, "Unpacking block number 11");
  tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

  // Check if pointer is not null, otherwise print the error and the block number
  if(tmp_ptr == 0 ){
    // std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(), _workers->back()) << std::endl;
    std::cout << "ERR: Error unpacking block number 11" << std::endl;
    std::cout << "ERR: Error value :" << _error_status << std::endl;
    return _error_status; }
    MTF7_DEBUG_MSG(std::cout, "Unpacked block number 11"); MTF7_DEBUG(std::cout, tmp_ptr);


    // for (block_operator_iterator iter = _workers -> begin(); 
    // 		iter != _workers -> end(); iter++){
    // 	if (_error_status != NO_ERROR) return _error_status;
    
    // 	emutf_block_operator * _tmp_block_operator = dynamic_cast<emutf_block_operator *> (*iter);
    
    // 	MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info");
    // 	_tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
    
    // 	MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
    // 	MTF7_DEBUG_MSG( std::cout, "Unpacking block number "); MTF7_DEBUG(std::cout, std::distance(_workers->begin(),iter));
    // 	tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);
    
    // 	// check if pointer is not null, otherwise print the error and the block number
    // 	if(tmp_ptr == 0 ){
    // 		std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(),iter) << std::endl;
    // 		std::cout << "ERR: Error value :" << _error_status << std::endl;
    // 		return _error_status;
    // 	}
    // 	MTF7_DEBUG_MSG(std::cout, "Unpacked. "); MTF7_DEBUG(std::cout, tmp_ptr);
    
    // }
    
    std::cout << "All blocks unpacked: testing values and _error_status:" << std::endl;
    // MTF7_DEBUG_MSG( std::cout, "All blocks unpacked: testing values and _error_status:");
    MTF7_DEBUG( std::cout, _unpacked_event_info -> _emutf_amc13_header_block -> _amc13_header_lv1_id );  
    MTF7_DEBUG( std::cout, _error_status);
    
    return _error_status;

}


//----------------------------------------------------------------------
const mtf7::word_64bit *mtf7::emutf_data_operator::pack(){
	MTF7_DEBUG_MSG(std::cout, "######### Packing ######### "); 

	unsigned long total_buffer_size = 0;

	std::vector<unsigned long> buffer_sizes;

	_error_status = NO_ERROR;

	MTF7_DEBUG(std::cout, _workers ->size());

	for (block_operator_iterator iter = _workers -> begin(); 
			iter != _workers -> end(); iter++){

		if (_error_status != NO_ERROR){ free_block_owned_buffers(); return 0; }

		emutf_block_operator *tmp_ptr = dynamic_cast<emutf_block_operator*>(*iter);

		MTF7_DEBUG_MSG(std::cout, "Set event info to pack");
		// MTF7_DEBUG( std::cout, _event_info -> _l1a);
		// MTF7_DEBUG( std::cout, _event_info -> _csc_me_bxn);
		// MTF7_DEBUG( std::cout, _event_info -> _amc13_header_lv1_id);
		MTF7_DEBUG( std::cout, typeid(_event_info).name());
		tmp_ptr -> set_event_info_to_pack ( _event_info ); // this is calling the emutf_block_operator

		MTF7_DEBUG_MSG( std::cout, "Packing block...");
		unsigned long temp = tmp_ptr -> pack ();
		MTF7_DEBUG_MSG( std::cout, "Packed");

		total_buffer_size += temp;
		buffer_sizes.push_back(temp);

	}

	MTF7_DEBUG( std::cout, total_buffer_size ); MTF7_DEBUG( std::cout, buffer_sizes.size() );
	for( unsigned i = 0; i < buffer_sizes.size(); ++i){ MTF7_DEBUG(std::cout, i); MTF7_DEBUG( std::cout, buffer_sizes.at(i)); }

	// todo: is this a memory leak? how do I free this buffer in this design?
	mtf7::word_64bit *buffer_ptr = (mtf7::word_64bit *) malloc( total_buffer_size << 3 ); // multipling the total buffer size by 8 to get bits. (size_t has to be in bytes). total_buffer_size is in 64bit words unit.

	std::vector <unsigned long>::iterator size_iter = buffer_sizes.begin();

	mtf7::word_64bit *temp_ptr = buffer_ptr;

	for (block_operator_iterator iter = _workers -> begin(); iter != _workers -> end(); iter++, size_iter++){
		memcpy(temp_ptr, (*iter) -> get_buffer_start_ptr(), (*size_iter) << 3);
		(*iter) -> free_own_buffer();
		temp_ptr += *size_iter;
	}

	MTF7_DEBUG_MSG( std::cout, "All blocks packed");
	MTF7_DEBUG( std::cout, *buffer_ptr);

	return buffer_ptr;

}

void mtf7::emutf_data_operator::free_block_owned_buffers(){

	for (block_operator_iterator iter = _workers -> begin(); iter != _workers -> end(); iter++)
		(*iter) -> free_own_buffer();

}


