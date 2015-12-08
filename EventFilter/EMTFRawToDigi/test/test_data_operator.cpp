#include "mtf7/emutf_header_block_operator.h"
#include "mtf7/emutf_event_generator.h"
#include "mtf7/emutf_data_operator.h"
#include "mtf7/emutf_operator_builder.h"
#include "mtf7/emutf_debug.h"
#include "mtf7/unity_test.h"
#include "mtf7/L1TMuonUpgradeTFCandProd.h"
#include <string>
#include <iostream>

#include "mtf7/block_operator.h"
#include <fstream>
#include <iostream>

int main(){
	int errorCounter = 0;
	/* 
	 * These commented out lines will create a text and binary file of the data
	 mtf7::event_generator mygen_output;
	//filename withuot extension
	std::string filename("test");
	mygen_output.setFileName(filename);
	// this will generate 10 events and write them in a text file and a binary file. 
	mygen_output.generateEvents(10);	
	*/
	for(int i = 1; i <= 2; i++){
		int temp = 0;
		//std::cout << "" << std::endl;
		std::cout << "Processing Event " << i << "\r" << std::flush;
		// ---------------
		// Testing packing and unpacking using data operator
		// ---------------
		// create the pointer to the event to pack and its error handler

		mtf7::event_generator mygen;
		mtf7::emutf_event * _generated_event = new mtf7::emutf_event();
		mtf7::error_value * err = new mtf7::error_value();
		// fill the event with random information
		mygen.generateEvent(i, _generated_event);
		const mtf7::emutf_event * event_to_pack = &*_generated_event;

		//  --- Testing packing ---
		const char* data_release = "test";

		mtf7::emutf_operator_builder * my_operator = new mtf7::emutf_operator_builder();
		mtf7::emutf_data_operator * my_data_operator = my_operator -> get_data_operator(data_release);
		// set the event to pack
		// you aways need to set_info_to_pack when you want to pack. look at emutf_block_operator.h
		my_data_operator -> set_event_info_to_pack(event_to_pack);
		// packing the event
		const mtf7::word_64bit * buffer_pointer = my_data_operator -> pack();

		//  This works! ------	


		//  --- Unpacking the packed event ---
		mtf7::emutf_event * _unpacked_event = new mtf7::emutf_event();
		// setting the event where the unpacked information will be stored
		my_data_operator -> set_unpacked_event_info_ptr(_unpacked_event);
		// // and now unpacking
		my_data_operator->unpack(buffer_pointer);

		MTF7_DEBUG( std::cout, _unpacked_event -> _l1a);
		MTF7_DEBUG( std::cout, _unpacked_event -> _amc13_header_lv1_id);
		MTF7_DEBUG( std::cout, _unpacked_event -> _csc_me_bxn);

		// --- Unity test		
		//repack
		const mtf7::emutf_event *unpack_1 = &*_unpacked_event;
		mtf7::emutf_operator_builder * my_operator2 = new mtf7::emutf_operator_builder();
		mtf7::emutf_data_operator * my_data_operator2 = my_operator2 -> get_data_operator(data_release);
		// set the event to pack
		// you aways need to set_info_to_pack when you want to pack. look at emutf_block_operator.h
		my_data_operator2 -> set_event_info_to_pack(_unpacked_event);
		// packing the event
		const mtf7::word_64bit * buffer_pointer2 = my_data_operator2 -> pack();

		//unpack again
		mtf7::emutf_event * _unpacked_event2 = new mtf7::emutf_event();
		// setting the event where the unpacked information will be stored
		my_data_operator2 -> set_unpacked_event_info_ptr(_unpacked_event2);
		// and now unpacking
		my_data_operator2->unpack(buffer_pointer2);
		const mtf7::emutf_event *unpack_2 = &*_unpacked_event2;

		//Perform Unity test
		mtf7::unity_test test;
		int errors = test.run(unpack_1, unpack_2, temp);
		errorCounter += errors;
		//  This works! ------
		mtf7::L1TMuonUpgradeTFCandProd::produce prod;
		void run = prod(unpack_1);

		delete my_operator;
		delete err;
		delete _generated_event;
		delete my_data_operator;
		delete _unpacked_event;

	}

	std::cout << "" << std::endl;
	if(errorCounter != 0 ){
		std::cout << "" << std::endl;
		std::cout << "There is a problem in the pack->unpack->pack->unpack chain!" << std::endl;
		std::cout << "                                  +-------------+" << std::endl;
	}
	std::cout << "There were " << errorCounter << " errors!" << std::endl;	
	return 0;

}
