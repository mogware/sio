#ifndef __SIO_OS_SYS_UTSNAME__
#define __SIO_OS_SYS_UTSNAME__

#include <string>

namespace sio
{
	/**
	* Returns the operating system name.
	*/
	std::string uname_sysname(void);

	/**
	* Returns the network name of this machine. 
	*/
	std::string uname_nodename(void);
}

#endif

