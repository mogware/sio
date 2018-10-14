#ifndef __SIO_SOCK_ADDR6__
#define __SIO_SOCK_ADDR6__

#include <cstdint>
#include <vector>
#include <cstring>

#include "sio.config.h"
#include "sio.os_errno.h"
#include "sio.os_arpa_inet.h"
#include "sio.os_sys_uio.h"
#include "sio.os_sys_socket.h"

namespace sio
{
	struct sock_addr6 : public ::sockaddr_in6
	{
		SIO_INLINE sock_addr6(void);
		SIO_INLINE sock_addr6(const std::uint8_t* addr, const std::uint16_t& port = 0);
		SIO_INLINE sock_addr6(const std::vector<std::uint8_t>& address,
			const std::uint16_t& port = 0);
		SIO_INLINE sock_addr6(const sockaddr_t* sa);
		SIO_INLINE sock_addr6(const sockaddr_in6_t* sa);
		SIO_INLINE std::uint16_t get_port(void) const;
		SIO_INLINE const std::uint8_t* get_addr(void) const;
		SIO_INLINE operator sockaddr_t* ();
		SIO_INLINE operator sockaddr_in6_t* ();
	};
}

#if defined(__SIO_INLINE__)
#include "sio.sock_addr6.inl"
#endif

#endif
