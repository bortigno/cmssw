#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event_generator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_header_block_operator.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include "math.h"
#include <bitset>

#include <typeinfo>



void mtf7::event_generator::setFileName( std::string _fileName ){

	fileName = _fileName;
	if(!fileName.find(".txt"))
		fileName += ".txt";

}

unsigned mtf7::event_generator::createMask(unsigned a, unsigned b){ //This method was taken from http://stackoverflow.com/questions/8011700/how-do-i-extract-specific-n-bits-of-a-32-bit-unsigned-integer-in-c
	unsigned r = 0;
	for(unsigned i=a; i<=b; i++){
		r |= 1 << i;
	}
	return r;
}

// generate a single event with event number i
void mtf7::event_generator::generateEvent( int event_number, emutf_event * unpacked_event ){

	mtf7::clear_emutf_event( unpacked_event );
	generateAMC13Header( event_number, unpacked_event );
	generateEventRecordHeader(event_number, unpacked_event);
	generateBlockOfCounters(unpacked_event);
	generateMEdataRecord( unpacked_event );
	generateRPCdataRecord( unpacked_event );
	generateSPoutputDataRecord( unpacked_event );
	generateEventRecordTrailer( unpacked_event );
	generateAMC13Trailer( unpacked_event );

}


void mtf7::event_generator::generateEvents( int nevents ){
std::cout << "Running 'generateEvents'" << std::endl;	
// initialize the rng with a seed from machine clock
	srand(time(NULL)); 

	std::string txt_ex(".txt");
	std::string bin_ex(".dat");
	// create an ascii output file
	std::ofstream asciiFile;
	asciiFile.open( (fileName+txt_ex).c_str() );
	if (!asciiFile.is_open()) std::cout << ("Error opening file") << std::endl;

	// create an binary output file
	std::ofstream binaryFile;
	binaryFile.open( (fileName+bin_ex).c_str(), std::ios::binary );
	if (!binaryFile.is_open()) std::cout << ("Error opening file") << std::endl;


	// set the seed using the machine clock
	srand(time(NULL));

	// set event number offset
	int offset = 3;

	emutf_event * unpacked_event = new emutf_event();
	std::cout << "Begin generating events..." << std::endl;
	for( int i = offset; i < nevents+offset; i++){ 
		std::cout << "Event Number: " << i << "\r" << std::flush;
		generateEvent(i, unpacked_event);

		// generateEventRecordHeader(i, unpacked_event);
		// generateBlockOfCounters(unpacked_event);

		//writing information in ascii file. todo: put a flag becuase you dont always want to write in ascii
		asciiFile << "-------------------------" << std::endl;
		asciiFile << "" << unpacked_event -> _l1a << std::endl;
		asciiFile << "" << unpacked_event -> _bxn << std::endl;
		asciiFile << "" << (int)unpacked_event -> _sp_ts << std::endl;
		asciiFile << "" << (int)unpacked_event -> _sp_ersv << std::endl;
		asciiFile << "" << (int)unpacked_event -> _sp_addr << std::endl;
		asciiFile << "" << (int)unpacked_event -> _tbin << std::endl;
		asciiFile << "" << unpacked_event -> _ddm << std::endl;
		asciiFile << "" << unpacked_event -> _spa << std::endl;
		asciiFile << "" << unpacked_event -> _rpca << std::endl;
		asciiFile << "" << unpacked_event -> _skip << std::endl;
		asciiFile << "" << unpacked_event -> _rdy << std::endl;
		asciiFile << "" << unpacked_event -> _bsy << std::endl;
		asciiFile << "" << unpacked_event -> _osy << std::endl;
		asciiFile << "" << unpacked_event -> _wof << std::endl;
		asciiFile << "" << unpacked_event -> _ME1a << std::endl;
		asciiFile << "" << unpacked_event -> _ME1b << std::endl;
		asciiFile << "" << unpacked_event -> _ME2 << std::endl;
		asciiFile << "" << unpacked_event -> _ME3 << std::endl;
		asciiFile << "" << unpacked_event -> _ME4 << std::endl;
		asciiFile << "" << unpacked_event -> _RPC << std::endl;
		asciiFile << "" << unpacked_event -> _TC << std::endl;
		asciiFile << "" << unpacked_event -> _OC << std::endl;

		asciiFile << "" << unpacked_event -> _csc_me_bxn << std::endl;
		// --------
		//write a binary file
		//every 64 bit word has to be written from right to left, so in order of 16bit words: d,c,b,a
		std::bitset<64> _64bit_word (0x0);
		//word d
		_64bit_word |= (0x9<<12);
		_64bit_word |= (unpacked_event->_bxn);
		//word c
		_64bit_word<<=16;
		_64bit_word |= (0x9<<12);
		//word b
		_64bit_word<<=16;
		_64bit_word |= (0x9<<12);
		_64bit_word |= (unpacked_event -> _l1a & 0xfff) >> 12;
		//word a
		_64bit_word<<=16;
		_64bit_word |= (0x9<<12);
		_64bit_word |= (unpacked_event -> _l1a & 0xffff);

		binaryFile.write((char*)&_64bit_word,sizeof(_64bit_word));

		std::bitset<64> _HD2 (0x0);
		//word d
		_HD2 |= (0xA<<12);
		_HD2 |= createMask(1,9) & unpacked_event->_ME1a; 
		//word c
		_HD2<<=16;
		_HD2 |= (0xA<<12);
		_HD2 |= (createMask(0,2) & unpacked_event->_tbin)<<8; // place these three bits on D10-D8
		_HD2 |= (unpacked_event->_ddm )<<7;
		_HD2 |= (unpacked_event->_spa )<<6;
		_HD2 |= (unpacked_event->_rpca)<<5;
		_HD2 |= (unpacked_event->_skip)<<4;
		_HD2 |= (unpacked_event->_rdy )<<3;
		_HD2 |= (unpacked_event->_bsy )<<2;
		_HD2 |= (unpacked_event->_osy )<<1;
		_HD2 |= (unpacked_event->_wof )<<0;
		//word b
		_HD2<<=16;
		_HD2 |= (0xA<<12);
		_HD2 |= (createMask(0,3) & unpacked_event ->_sp_ts)<<8;
		_HD2 |= (createMask(0,2) & unpacked_event ->_sp_ersv)<<5;
		_HD2 |= (createMask(0,4) & unpacked_event ->_sp_addr)<<0;
		//word a
		_HD2<<=16;
		_HD2 |= (0xA<<12);

		binaryFile.write((char*)&_HD2,sizeof(_HD2));

		std::bitset<64> _HD3 (0x0);
		//word d
		_HD3 |= (createMask(19,24) & unpacked_event->_RPC)<<9;
		_HD3 |= (createMask(1,9) & unpacked_event->_ME4);
		//word c
		_HD3<<=16;
		_HD3 |= (createMask(13,18) & unpacked_event->_RPC)<<9;
		_HD3 |= (createMask(1,9) & unpacked_event->_ME3);
		//word b
		_HD3<<=16;
		_HD3 |= (createMask(7,12) & unpacked_event->_RPC)<<9;
		_HD3 |= (createMask(1,9) & unpacked_event->_ME2);
		//word a
		_HD3<<=16;
		_HD3 |= (createMask(15,15) & 0xffff)<<15;
		_HD3 |= (createMask(1,6) & unpacked_event->_RPC)<<9;
		_HD3 |= (createMask(1,9) & unpacked_event->_ME1b);

		binaryFile.write((char*)&_HD3,sizeof(_HD3));


		//Block of Counters     
		std::bitset<64> _BC (0x0);
		//word d
		_BC |= createMask(15,29) & unpacked_event->_OC;

		//word c
		_BC<<=16;
		_BC |= createMask(0,14) & unpacked_event->_OC;
		//word b
		_BC<<=16;
		_BC |= (createMask(15,15) & 0xffff)<<15;
		_BC |= createMask(15,29) & unpacked_event->_TC;

		//word a
		_BC<<=16;
		_BC |= createMask(0,14) & unpacked_event->_TC;

		binaryFile.write((char*)&_BC,sizeof(_BC));

		//MEX Data Record
		std::bitset<64> _ME (0x0);
		//word d
		_ME |= (unpacked_event->_csc_afef )<<14;
		_ME |= (unpacked_event->_csc_se )<<13;
		_ME |= (unpacked_event->_csc_sm )<<12;
		_ME |= (createMask(0,3) & unpacked_event->_csc_epc)<<8;
		_ME |= (unpacked_event->_csc_af)<<7;
		_ME |= (createMask(0,2) & unpacked_event->_csc_station)<<4;
		_ME |= (unpacked_event->_csc_vp)<<3;
		_ME |= (createMask(0,2) & unpacked_event->_csc_tbin_num)<<0;

		//word c
		_ME<<=16;
		_ME |= (unpacked_event->_csc_afff)<<14;
		_ME |= (unpacked_event->_csc_cik)<<13;
		_ME |= (unpacked_event->_csc_nit)<<12;
		_ME |= (createMask(0,11) & unpacked_event->_csc_me_bxn)<<0;
		//word b
		_ME<<=16;
		_ME |= (createMask(15,15) & 0xffff)<<15;
		_ME |= (unpacked_event->_csc_bc0)<<14;
		_ME |= (unpacked_event->_csc_bxe)<<13;
		_ME |= (unpacked_event->_csc_lr)<<12;
		_ME |= (createMask(0,3) & unpacked_event->_csc_id)<<8;
		_ME |= (createMask(0,7) & unpacked_event->_csc_clct_key_half_strip)<<0;

		//word a
		_ME<<=16;
		_ME |= (createMask(15,15) & 0xffff)<<15;
		_ME |= (createMask(0,6) & unpacked_event->_csc_key_wire_group)<<8;
		_ME |= (createMask(0,3) & unpacked_event->_csc_quality)<<4;
		_ME |= (createMask(0,3) & unpacked_event->_csc_clct_pattern)<<0;

		binaryFile.write((char*)&_ME,sizeof(_ME));

		//RPC Data Record

		std::bitset<64> _RPC (0x0);
		//word d
		_RPC |= (createMask(0,2) & unpacked_event->_rpc_tbin)<<0;

		//word c
		_RPC<<=16;
		_RPC |= (createMask(15,15) & 0xffff)<<15;
		_RPC |= (unpacked_event->_rpc_bc0)<<14;
		_RPC |= (createMask(0,11) & unpacked_event->_rpc_bxn)<<0;
		//word b
		_RPC<<=16;
		_RPC |= (createMask(0,5) & unpacked_event->_rpc_bcn)<<8;
		_RPC |= (unpacked_event->_rpc_eod)<<7;
		_RPC |= (createMask(0,1) & unpacked_event->_rpc_lb)<<5;
		_RPC |= (createMask(0,4) & unpacked_event->_rpc_link_number)<<0;
		//word a
		_RPC<<=16;
		_RPC |= (createMask(0,2) & unpacked_event->_rpc_prt_delay)<<12;
		_RPC |= (createMask(0,3) & unpacked_event->_rpc_partition_num)<<8;
		_RPC |= (createMask(0,7) & unpacked_event->_rpc_partition_data)<<0;

		binaryFile.write((char*)&_RPC,sizeof(_RPC));

		//SP Output Data Record

		std::bitset<64> _SP1 (0x0);
		//word d
		_SP1 |= (createMask(0,2) & unpacked_event->_track_bx)<<9;
		_SP1 |= (createMask(0,8) & unpacked_event->_track_pt)<<0;

		//word c
		_SP1<<=16;
		_SP1 |= (createMask(15,15) & 0xffff)<<15;
		_SP1 |= (createMask(0,3) & unpacked_event->_track_quality)<<9;
		std::cout << "unpacked_event->_track_eta: " << unpacked_event->_track_eta << std::endl;
		_SP1 |= (createMask(0,8) & unpacked_event->_track_eta)<<0;
		//word b
		_SP1<<=16;
		_SP1 |= (unpacked_event->_track_se)<<13;
		_SP1 |= (unpacked_event->_track_bc0)<<12;
		_SP1 |= (createMask(0,11) & unpacked_event->_track_phi_outer)<<0;
		//word a
		_SP1<<=16;
		_SP1 |= (createMask(15,15) & 0xffff)<<15;
		_SP1 |= (unpacked_event->_track_hl)<<14;
		_SP1 |= (unpacked_event->_track_c)<<13;
		_SP1 |= (unpacked_event->_track_vc)<<12;
		_SP1 |= (createMask(0,11) & unpacked_event->_track_phi_inner)<<0;

		binaryFile.write((char*)&_SP1,sizeof(_SP1));

		std::bitset<64> _SP2 (0x0);
		//word d
		_SP2 |= (createMask(15,29) & unpacked_event->_track_pt_lut_address)<<0;

		//word c
		_SP2<<=16;
		_SP2 |= (createMask(15,15) & 0xffff)<<15;
		_SP2 |= (createMask(0,14) & unpacked_event->_track_pt_lut_address)<<0;
		//word b
		_SP2<<=16;
		_SP2 |= (createMask(15,15) & 0xffff)<<15;
		_SP2 |= (createMask(0,2) & unpacked_event->_track_tbin_num)<<12;
		_SP2 |= (createMask(0,2) & unpacked_event->_track_me4_tbin)<<9;
		_SP2 |= (createMask(0,2) & unpacked_event->_track_me3_tbin)<<6;
		_SP2 |= (createMask(0,2) & unpacked_event->_track_me2_tbin)<<3;
		_SP2 |= (createMask(0,2) & unpacked_event->_track_me1_tbin)<<0;
		//word a
		_SP2<<=16;
		_SP2 |= (createMask(0,1) & unpacked_event->_track_me4_id)<<7;
		_SP2 |= (createMask(0,1) & unpacked_event->_track_me3_id)<<5;
		_SP2 |= (createMask(0,1) & unpacked_event->_track_me2_id)<<3;
		_SP2 |= (createMask(0,2) & unpacked_event->_track_me1_id)<<0;

		binaryFile.write((char*)&_SP2,sizeof(_SP2));

		//Event Record Trailer
		std::bitset<64> _TR1 (0x0);
		//word d
		_TR1 |= (0xF<<12);
		_TR1 |= (createMask(0,11) & unpacked_event->_trailer_spcrs_scc)<<0;

		//word c
		_TR1<<=16;
		_TR1 |= (0xF<<12);
		_TR1 |= (createMask(8,15) & unpacked_event->_trailer_ddcrs_bid)<<0;
		//word b
		_TR1<<=16;
		_TR1 |= (0xF<<12);
		_TR1 |= (createMask(4,7) & unpacked_event->_trailer_ddcrs_lf)<<8;
		_TR1 |= (unpacked_event->_trailer_lfff)<<7;
		_TR1 |= (0x7<<4);
		_TR1 |= (0xF<<0);

		//word a
		_TR1<<=16;
		_TR1 |= (0xF<<12);
		_TR1 |= (createMask(0,3) & unpacked_event->_trailer_ddcrs_lf)<<8;
		_TR1 |= (createMask(0,7) & unpacked_event->_trailer_l1a)<<0;

		binaryFile.write((char*)&_TR1,sizeof(_TR1));

		std::bitset<64> _TR2 (0x0);
		//word d
		_TR2 |= (0xE<<12);
		_TR2 |= (unpacked_event->_trailer_hp)<<11;
		_TR2 |= (createMask(11,21) & unpacked_event->_trailer_crc22)<<0;
		//word c
		_TR2<<=16;
		_TR2 |= (0xE<<12);
		_TR2 |= (unpacked_event->_trailer_lp)<<11;
		_TR2 |= (createMask(0,10) & unpacked_event->_trailer_crc22)<<0;
		//word b
		_TR2<<=16;
		_TR2 |= (0xE<<12);
		_TR2 |= (createMask(0,3) & unpacked_event->_trailer_sp_ladr)<<8;
		_TR2 |= (createMask(0,2) & unpacked_event->_trailer_sp_ersv)<<5;
		_TR2 |= (createMask(0,4) & unpacked_event->_trailer_sp_padr)<<8;

		//word a
		_TR2<<=16;
		_TR2 |= (0xE<<12);
		_TR2 |= (createMask(0,4) & unpacked_event->_trailer_ddcrs_bid)<<0;

		binaryFile.write((char*)&_TR2,sizeof(_TR2));
	}
	std::cout << "" << std::endl;
	std::cout << "Event generation completed" << std::endl;
	// close an ascii file
	asciiFile.close();

	// close a bniary file
	binaryFile.close();

	delete unpacked_event;

}

unsigned long mtf7::event_generator::generateInt( int nbits ){

	unsigned long num = 0;
	int max = (int)pow(2,nbits); // max numbers allowed with nbits
	num = rand() % max; // generate a random integer between 0 and (max-1)
	return num;

}


void mtf7::event_generator::generateAMC13Header( int i, emutf_event * _event ){

	// CDF header
	_event -> _amc13_header_evt_ty = generateInt(4);
	_event -> _amc13_header_lv1_id = generateInt(24);
	_event -> _amc13_header_bx_id = generateInt(12);
	_event -> _amc13_header_source_id = generateInt(12);
	_event -> _amc13_header_fov = generateInt(4);
	_event -> _amc13_header_h = generateInt(1);
	_event -> _amc13_header_x = generateInt(1);

	// AMC13 header
	_event -> _amc13_header_ufov = generateInt(4);
	_event -> _amc13_header_res = generateInt(4);
	_event -> _amc13_header_namc = generateInt(4);
	_event -> _amc13_header_orn = generateInt(32);

}

void mtf7::event_generator::generateEventRecordHeader( int i, emutf_event * _event){

	// generate L1A
	_event -> _l1a = i;

	// generate event bunch crossing number
	_event -> _bxn = i;

	// generate sp information
	_event -> _sp_ts = generateInt(4); // 4 in the dataformat
	_event -> _sp_ersv = generateInt(3); // 3 in the df
	_event -> _sp_addr = generateInt(5); // 5 in the df
	//std::cout << "sp_addr = " << _event -> _sp_addr << std::endl;
	//std::cout << "Size of sp_addr = " << sizeof(_event -> _sp_addr) << std::endl;
	_event -> _tbin = generateInt(3); // 3 in the df

	// generate status bits
	_event -> _ddm  = generateInt(1);
	_event -> _spa = generateInt(1);
	_event -> _rpca = generateInt(1);
	_event -> _skip = generateInt(1);
	_event -> _rdy = generateInt(1);
	_event -> _bsy = generateInt(1);
	_event -> _osy = generateInt(1);
	_event -> _wof = generateInt(1); 

	// generate csc information
	_event -> _ME1a = generateInt(9);
	_event -> _ME1b = generateInt(9);
	_event -> _ME2 = generateInt(9);
	_event -> _ME3 = generateInt(9);
	_event -> _ME4 = generateInt(9);

	// generate rpc information
	_event -> _RPC = generateInt(24);

}


void mtf7::event_generator::generateBlockOfCounters( emutf_event * _event ){
	_event -> _TC = generateInt(30);
	_event -> _OC = generateInt(30);
}


void mtf7::event_generator::generateMEdataRecord( emutf_event * _event ){

	_event -> _csc_me_bxn = generateInt(16);

	_event -> _csc_key_wire_group = generateInt(8);
	_event -> _csc_clct_key_half_strip = generateInt(8);
	_event -> _csc_quality = generateInt(8);
	_event -> _csc_clct_pattern = generateInt(8);
	_event -> _csc_id = generateInt(8);
	_event -> _csc_epc = generateInt(8);
	_event -> _csc_station = generateInt(8);
	_event -> _csc_tbin_num = generateInt(8);

	_event -> _csc_bc0 = generateInt(1);
	_event -> _csc_bxe = generateInt(1);
	_event -> _csc_lr = generateInt(1);
	_event -> _csc_afff = generateInt(1);
	_event -> _csc_cik = generateInt(1);
	_event -> _csc_nit = generateInt(1);
	_event -> _csc_afef = generateInt(1);
	_event -> _csc_se = generateInt(1);
	_event -> _csc_sm = generateInt(1);
	_event -> _csc_af = generateInt(1);
	_event -> _csc_vp = generateInt(1);
}


void mtf7::event_generator::generateRPCdataRecord( emutf_event * _event ){

	_event -> _rpc_prt_delay = generateInt(8);
	_event -> _rpc_partition_num = generateInt(8);
	_event -> _rpc_partition_data = generateInt(8);
	_event -> _rpc_bcn = generateInt(8);
	_event -> _rpc_lb = generateInt(8);
	_event -> _rpc_link_number = generateInt(8);
	_event -> _rpc_bxn = generateInt(8);
	_event -> _rpc_tbin = generateInt(8);

	_event -> _rpc_eod = generateInt(1);
	_event -> _rpc_bc0 = generateInt(1);

}


void mtf7::event_generator::generateSPoutputDataRecord( emutf_event * _event ){
	_event -> _track_pt_lut_address = generateInt(32);
	_event -> _track_phi_inner = generateInt(16);
	_event -> _track_phi_outer = generateInt(16);
	_event -> _track_eta = generateInt(16);
	_event -> _track_pt = generateInt(16);

	_event -> _track_quality = generateInt(8);
	_event -> _track_bx = generateInt(8);
	_event -> _track_me4_id = generateInt(8);
	_event -> _track_me3_id = generateInt(8);
	_event -> _track_me2_id = generateInt(8);
	_event -> _track_me1_id = generateInt(8);
	_event -> _track_tbin_num = generateInt(8);
	_event -> _track_me4_tbin = generateInt(8);
	_event -> _track_me3_tbin = generateInt(8);
	_event -> _track_me2_tbin = generateInt(8);
	_event -> _track_me1_tbin = generateInt(8);

	_event -> _track_hl = generateInt(1);
	_event -> _track_c = generateInt(1);
	_event -> _track_vc = generateInt(1);
	_event -> _track_se = generateInt(1);
	_event -> _track_bc0 = generateInt(1);

}

void mtf7::event_generator::generateEventRecordTrailer( emutf_event * _event ){

	_event -> _trailer_crc22 = generateInt(32);

	_event -> _trailer_ddcrs_bid = generateInt(16);
	_event -> _trailer_spcrs_scc = generateInt(16);

	_event -> _trailer_l1a = generateInt(8);
	_event -> _trailer_yy = generateInt(8);
	_event -> _trailer_mm = generateInt(8);
	_event -> _trailer_dd = generateInt(8);
	_event -> _trailer_sp_ladr = generateInt(8);
	_event -> _trailer_sp_ersv = generateInt(8);
	_event -> _trailer_sp_padr = generateInt(8);
	_event -> _trailer_ddcrs_lf = generateInt(8);

	_event -> _trailer_lfff = generateInt(1);
	_event -> _trailer_bb = generateInt(1);
	_event -> _trailer_lp = generateInt(1);
	_event -> _trailer_hp = generateInt(1);

}


void mtf7::event_generator::generateAMC13Trailer( emutf_event * _event ){

	// AMC13 Trailer

	_event -> _amc13_trailer_evt_lgth = generateInt(24);
	_event -> _amc13_trailer_crc16 = generateInt(16);
	_event -> _amc13_trailer_c = generateInt(1);
	_event -> _amc13_trailer_f = generateInt(1);
	_event -> _amc13_trailer_evt_stat = generateInt(4);
	_event -> _amc13_trailer_tts = generateInt(4);
	_event -> _amc13_trailer_t = generateInt(1);
	_event -> _amc13_trailer_r = generateInt(1);


}

