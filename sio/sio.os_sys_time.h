#ifndef __SIO_OS_SYS_TIME__
#define __SIO_OS_SYS_TIME__

#include "sio.config.h"

namespace sio
{
	typedef struct ::timeval timeval_t;

	/**
	* Returns current time, expressed as seconds and microseconds
	* since the Epoch.
	*/
	int gettimeofday(timeval_t* tv, void *);
}


#endif

