#ifndef __SIO_SOCK_ADDR4__
#define __SIO_SOCK_ADDR4__

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
	struct sock_addr4 : public ::sockaddr_in
	{
		SIO_INLINE sock_addr4(void);
		SIO_INLINE sock_addr4(const std::uint32_t& addr, const std::uint16_t& port = 0);
		SIO_INLINE sock_addr4(const std::vector<std::uint8_t>& address,
			const std::uint16_t& port = 0);
		SIO_INLINE sock_addr4(const sockaddr_t* sa);
		SIO_INLINE sock_addr4(const sockaddr_in_t* sa);
		SIO_INLINE std::uint16_t get_port(void) const;
		SIO_INLINE std::uint32_t get_addr(void) const;
		SIO_INLINE operator sockaddr_t* ();
		SIO_INLINE operator sockaddr_in_t* ();
	};
}

#if defined(__SIO_INLINE__)
#include "sio.sock_addr4.inl"
#endif

#endif

