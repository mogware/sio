#ifndef __SIO_OS_NETDB__
#define __SIO_OS_NETDB__

#include "sio.config.h"
#include "sio.os_errno.h"
#include "sio.os_sys_uio.h"
#include "sio.os_sys_socket.h"

namespace sio
{
	/**
	* Returns the host name from an address and service name from a
	* port number.
	*/
	SIO_INLINE void get_nameinfo(const sockaddr_t* addr, const int& len,
		char* host, const int& hostlen,
		char* serv, const int& servlen, const int& flags);

	/**
	* Returns the address from a hostname and service name.
	*/
	SIO_INLINE void get_addrinfo(const char* node, const char* serv,
		const addrinfo_t* hints, addrinfo_t** result);

	/**
	* Frees address information that the getaddrinfo function dynamically
	* allocates.
	*/
	SIO_INLINE void free_addrinfo(addrinfo_t* result);
}

#if defined(__SIO_INLINE__)
#include "sio.os_netdb.inl"
#endif

#endif
