#include "mtf7/emutf_header_block_operator.h"
#include "mtf7/emutf_event_generator.h"
#include <string>
#include <iostream>

int main(){
	
	mtf7::event_generator mygen;

	//filename withuot extension
	std::string filename("test");
	mygen.setFileName(filename);
	// this will generate 10 events and write them in a text file and a binary file. 
	mygen.generateEvents(10);


	// ---------------
	// Testing packing
	// ---------------

	// create the pointer to the event to pack and its error handler
	mtf7::emutf_event * event_to_pack = new mtf7::emutf_event();
	mtf7::error_value * err = new mtf7::error_value();
	// fill the event with random information
	mygen.generateEvent(1, event_to_pack);
	//std::cout << "l1a = " << event_to_pack -> _l1a << std::endl;

	// create the operator for the header block
	mtf7::emutf_header_block_operator * my_operator = new mtf7::emutf_header_block_operator( err, event_to_pack );
	// set the event to pack
	// you aways need to set_info_to_pack when you want to pack. look at emutf_block_operator.h
	my_operator -> set_event_info_to_pack(event_to_pack);
	// packing the event 
	my_operator -> pack();

	return 0;

}
