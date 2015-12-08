#ifndef __EMUTF_EVENT_GENERATOR__
#define __EMUTF_EVENT_GENERATOR__

#include <string>
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event.h"

namespace mtf7{

	class event_generator{

	public:

		void setFileName( std::string fileName );

		void generateEvent( int _event_number, mtf7::emutf_event * unpacked_event );

		void generateEvents( int nevents );

		void setSeed( int seed );

		void generateAMC13Header( int i, emutf_event * unpacked_event );

		void generateEventRecordHeader( int i, emutf_event * unpacked_event); 

		void generateBlockOfCounters( emutf_event * unpacked_event);

		void generateMEdataRecord( emutf_event * unpacked_event );

		void generateRPCdataRecord( emutf_event * unpacked_event );

		void generateSPoutputDataRecord( emutf_event * unpacked_event );

		void generateEventRecordTrailer( emutf_event * unpacked_event );

		void generateAMC13Trailer( emutf_event * unpacked_event );

	private:

		std::string fileName;

		// generate an int of nbits (with a max of 64 bit)
		unsigned long generateInt( int nbits );

		int generateInitialL1A();

		unsigned createMask(unsigned a, unsigned b);
	};

}

#endif
