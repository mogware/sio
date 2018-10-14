SIO_INLINE sio::sock_addr6::sock_addr6(void)
{
	std::memset(this, 0, sizeof(sockaddr_in6_t));
	sin6_family = AF_INET6;
}

SIO_INLINE sio::sock_addr6::sock_addr6(const std::uint8_t* addr, const std::uint16_t& port)
	: sock_addr6()
{
	sin6_port = sio::htons(port);
	std::memcpy(sin6_addr.s6_addr, addr, sizeof(sin6_addr.s6_addr));
}

SIO_INLINE sio::sock_addr6::sock_addr6(const std::vector<std::uint8_t>& address,
	const std::uint16_t& port)
	: sock_addr6()
{
	std::uint8_t addr[16];
	bool valid = address.size() == 16;
	for (int i = 0; i < 16; i++)
		addr[i] = valid ? address[i] : 0;
	sin6_port = sio::htons(port);
	std::memcpy(sin6_addr.s6_addr, addr, sizeof(sin6_addr.s6_addr));
}

SIO_INLINE sio::sock_addr6::sock_addr6(const sockaddr_t* sa)
	: sock_addr6()
{
	std::memcpy(this, sa, sizeof(sockaddr_in6_t));
}

SIO_INLINE sio::sock_addr6::sock_addr6(const sockaddr_in6_t* sa)
	: sock_addr6()
{
	std::memcpy(this, sa, sizeof(sockaddr_in6_t));
}

SIO_INLINE std::uint16_t sio::sock_addr6::get_port(void) const
{
	return sio::htons(sin6_port);
}

SIO_INLINE const std::uint8_t* sio::sock_addr6::get_addr(void) const
{
	return sin6_addr.s6_addr;
}

SIO_INLINE sio::sock_addr6::operator sio::sockaddr_t* ()
{
	return reinterpret_cast<sockaddr_t*>(this);
}

SIO_INLINE sio::sock_addr6::operator sio::sockaddr_in6_t* ()
{
	return reinterpret_cast<sockaddr_in6_t*>(this);
}

