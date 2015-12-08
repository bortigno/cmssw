#ifndef __MTF7_EMUTF_DATA_OPERATOR__
#define __MTF7_EMUTF_DATA_OPERATOR__

#include "EventFilter/EMTFRawToDigi/include/mtf7/data_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_event.h"
#include <iostream>
#include "EventFilter/EMTFRawToDigi/include/mtf7/emutf_block_operator.h"
#include "EventFilter/EMTFRawToDigi/include/mtf7/types.h"


namespace mtf7{

	class emutf_data_operator: public data_operator {

		public:

			emutf_data_operator( const char *dataformat_release );

			virtual error_value       unpack( const word_64bit *buffer );
			virtual const word_64bit *pack();

			void  set_event_info_to_pack( const emutf_event *event_info ) { _event_info = event_info; }
			void  set_unpacked_event_info_ptr( emutf_event *event_info ) { _unpacked_event_info = event_info; }

			const emutf_event   *get_event_info() { return _event_info; }
		protected:

			void free_block_owned_buffers();

			const emutf_event *_event_info;
			emutf_event * _unpacked_event_info;

	};

}

#endif
