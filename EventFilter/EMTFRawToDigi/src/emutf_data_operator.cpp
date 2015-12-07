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
	MTF7_DEBUG_MSG(std::cout, "######### Unpacking ######### "); 
	const word_64bit *tmp_ptr = buffer;

    // AMC13/CDF header
    emutf_block_operator * _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->front());
    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info");
    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
    MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
    // MTF7_DEBUG_MSG( std::cout, "Unpacking block number :"); MTF7_DEBUG(std::cout, std::distance(_workers->begin(),_workers->at(0)));
    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

    // check if pointer is not null, otherwise print the error and the block number
    if(tmp_ptr == 0 ){
            // std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(),_workerd->at(0)) << std::endl;
            std::cout << "ERR: Error value :" << _error_status << std::endl;
            return _error_status;
    }
    MTF7_DEBUG_MSG(std::cout, "Unpacked. "); MTF7_DEBUG(std::cout, tmp_ptr);

    MTF7_DEBUG_MSG(std::cout, "Number of AMC sending data to the AMC13 : "); MTF7_DEBUG(std::cout, (int)_unpacked_event_info -> _emutf_amc13_header_block -> _amc13_header_namc);

    // number of MTF7 boards sending data to the AMC13.
    unsigned int nAMC = (unsigned int)_unpacked_event_info -> _emutf_amc13_header_block -> _amc13_header_namc;
    std::cout << "nAMC important for vector assignmemt : " << nAMC << std::endl;

    // loop over all prepayloads headers present in the event
    for ( unsigned int _sp = 0; _sp < nAMC ; _sp++ ){
		_tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at(1)); // at 1 we gave the preheader.
	    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for preheader");
	    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
	    MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
	    // MTF7_DEBUG_MSG( std::cout, "Unpacking block number :"); MTF7_DEBUG(std::cout, std::distance(_workers->begin(),_workers->at(1)));
	    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

	    // check if pointer is not null, otherwise print the error and the block number
	    if(tmp_ptr == 0 ){
	            // std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(),_workerd->at(0)) << std::endl;
	            std::cout << "ERR: Error value :" << _error_status << std::endl;
	            return _error_status;
	    }
	    MTF7_DEBUG_MSG(std::cout, "Unpacked. "); MTF7_DEBUG(std::cout, tmp_ptr);
	}

    // all preheader give information about the size of the payload. If size if 3 or less there is no payload (zero suppression).
    // if size is > 3 then there are tracks to unpack from that specific board.

   for( unsigned int _sp = 0; _sp < (unsigned int)_unpacked_event_info -> _emutf_prepayload_header_block_vector.size() ; _sp++ )
      std::cout << "@INFO: Unpacked event info -> _prepayload_amcn_size[" << _sp << "] = " << (unsigned int) _unpacked_event_info -> _emutf_prepayload_header_block_vector.at(_sp) -> _prepayload_amcn_size << std::endl;

    // loop over all AMC13 payloads present in the event
    for ( unsigned int _sp = 0; _sp < nAMC ; _sp++ ){

		for (block_operator_iterator iter = _workers -> begin()+2; // the amc13 header and the preheader have been already unpacked 
				iter != _workers->end()-2 ; iter++){ // the amc13 trailer and the posttrailer will be unpacked at the very end
			if (_error_status != NO_ERROR) return _error_status;

		    _tmp_block_operator = dynamic_cast<emutf_block_operator *> (*iter);
		    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for payloads");
		    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
		    MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
		    MTF7_DEBUG_MSG( std::cout, "Unpacking block number :"); MTF7_DEBUG(std::cout, std::distance(_workers->begin(),iter));
		    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

		    // check if pointer is not null, otherwise print the error and the block number
		    if(tmp_ptr == 0 ){
		            // std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(),_workerd->at(0)) << std::endl;
		            std::cout << "ERR: Error value :" << _error_status << std::endl;
		            return _error_status;
		    }
		    MTF7_DEBUG_MSG(std::cout, "Unpacked. "); MTF7_DEBUG(std::cout, tmp_ptr);
    	          
                    // after unpacker the header check the size of the amc. if it is 3 go directly to the event trailer.
                    if (((unsigned int) _unpacked_event_info -> _emutf_prepayload_header_block_vector.at(_sp) -> _prepayload_amcn_size) < 4 && std::distance(_workers->begin(),iter) < 3)
			iter = _workers -> end()-4; // remember that it will add one at next iteration.       
                        MTF7_DEBUG_MSG( std::cout, "Empty SP. Going to the amc trailer (block 9).");
		}

    }	

    std::cout << "worker size : " << _workers->size() << std::endl;

    // Postpayload trailer
    _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->at(_workers->size()-2));
    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for postpayload trailer");
    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
    MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
    MTF7_DEBUG_MSG( std::cout, "Unpacking postpayload trailer");
    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

    // check if pointer is not null, otherwise print the error and the block number
    if(tmp_ptr == 0 ){
            // std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(),_workerd->at(0)) << std::endl;
            std::cout << "ERR: Error value :" << _error_status << std::endl;
            return _error_status;
    }
    MTF7_DEBUG_MSG(std::cout, "Unpacked. "); MTF7_DEBUG(std::cout, tmp_ptr);

    // AMC13/CDF trailer
    _tmp_block_operator = dynamic_cast<emutf_block_operator *> (_workers->back());
    MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info for trailer");
    _tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );
    MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
    MTF7_DEBUG_MSG( std::cout, "Unpacking AMC13 triler");
    tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

    // check if pointer is not null, otherwise print the error and the block number
    if(tmp_ptr == 0 ){
            // std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(),_workerd->at(0)) << std::endl;
            std::cout << "ERR: Error value :" << _error_status << std::endl;
            return _error_status;
    }
    MTF7_DEBUG_MSG(std::cout, "Unpacked. "); MTF7_DEBUG(std::cout, tmp_ptr);


	// for (block_operator_iterator iter = _workers -> begin(); 
	// 		iter != _workers -> end(); iter++){
	// 	if (_error_status != NO_ERROR) return _error_status;

	// 	emutf_block_operator * _tmp_block_operator = dynamic_cast<emutf_block_operator *> (*iter);

	// 	MTF7_DEBUG_MSG( std::cout, "Setting _unpacked_event_info");
	// 	_tmp_block_operator -> set_unpacked_event_info_ptr( _unpacked_event_info );

	// 	MTF7_DEBUG( std::cout, tmp_ptr ); MTF7_DEBUG( std::cout, *tmp_ptr );
	// 	MTF7_DEBUG_MSG( std::cout, "Unpacking block number :"); MTF7_DEBUG(std::cout, std::distance(_workers->begin(),iter));
	// 	tmp_ptr = _tmp_block_operator -> unpack (tmp_ptr);

	// 	// check if pointer is not null, otherwise print the error and the block number
	// 	if(tmp_ptr == 0 ){
	// 		std::cout << "ERR: Error unpacking block number " << std::distance(_workers->begin(),iter) << std::endl;
	// 		std::cout << "ERR: Error value :" << _error_status << std::endl;
	// 		return _error_status;
	// 	}
	// 	MTF7_DEBUG_MSG(std::cout, "Unpacked. "); MTF7_DEBUG(std::cout, tmp_ptr);

	// }

	MTF7_DEBUG_MSG( std::cout, "All blocks unpacked: testing values and _error_status:");
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


