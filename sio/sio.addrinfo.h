#ifndef __SIO_ADDRINFO__
#define __SIO_ADDRINFO

#include <cstring>

#include "sio.config.h"
#include "sio.os_errno.h"
#include "sio.os_sys_uio.h"
#include "sio.os_sys_socket.h"

namespace sio
{
	struct addrinfo : public ::addrinfo
	{
		SIO_INLINE addrinfo(void);
		SIO_INLINE operator addrinfo_t* ();
	};
}

#if defined(__SIO_INLINE__)
#include "sio.addrinfo.inl"
#endif

#endif
