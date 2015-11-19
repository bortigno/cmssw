#ifdef DEBUG_BUILD
	#define MTF7_DEBUG( os, var ) \
  		(os) << "DBG: " << __FILE__ << "(" << __LINE__ << ") "\
    	<< #var << " = [" << (var) << "]" << std::endl
    #define MTF7_DEBUG_MSG(os, msg) \
    	(os) << "DBG: " << __FILE__ << "(" << __LINE__ << ") "\
    	<< msg << std::endl
#else
    #define MTF7_DEBUG(os, var) do {} while (0)
    #define MTF7_DEBUG_MSG(os, msg) do {} while (0)
#endif