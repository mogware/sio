SIO_INLINE sio::sock_addr4::sock_addr4(void)
{
	std::memset(this, 0, sizeof(sockaddr_in_t));
	sin_family = AF_INET;
}

SIO_INLINE sio::sock_addr4::sock_addr4(const std::uint32_t& addr, const std::uint16_t& port)
	: sock_addr4()
{
	sin_addr.s_addr = sio::htonl(addr);
	sin_port = sio::htons(port);
}

SIO_INLINE sio::sock_addr4::sock_addr4(const std::vector<std::uint8_t>& address,
		const std::uint16_t& port)
	: sock_addr4()
{
	std::uint32_t addr = 0;
	if (address.size() == 4) {
		addr = address[3] & 0xFF;
		addr |= ((address[2] << 8) & 0xFF00);
		addr |= ((address[1] << 16) & 0xFF0000);
		addr |= ((address[0] << 24) & 0xFF000000);
	}
	sin_addr.s_addr = sio::htonl(addr);
	sin_port = sio::htons(port);
}

SIO_INLINE sio::sock_addr4::sock_addr4(const sockaddr_t* sa)
	: sock_addr4()
{
	std::memcpy(this, sa, sizeof(sockaddr_in_t));
}

SIO_INLINE sio::sock_addr4::sock_addr4(const sockaddr_in_t* sa)
	: sock_addr4()
{
	std::memcpy(this, sa, sizeof(sockaddr_in_t));
}

SIO_INLINE std::uint16_t sio::sock_addr4::get_port(void) const
{
	return sio::htons(sin_port);
}

SIO_INLINE std::uint32_t sio::sock_addr4::get_addr(void) const
{
	return sio::htonl(sin_addr.s_addr);
}

SIO_INLINE sio::sock_addr4::operator sio::sockaddr_t* ()
{
	return reinterpret_cast<sockaddr_t*>(this);
}

SIO_INLINE sio::sock_addr4::operator sio::sockaddr_in_t* ()
{
	return reinterpret_cast<sockaddr_in_t*>(this);
}
