SIO_INLINE void sio::get_nameinfo(const sio::sockaddr_t* sa,
		const int& salen, char* host, const int& hostlen,
		char* serv, const int& servlen, const int& flags)
{
	if (::getnameinfo(sa, salen, host, hostlen, serv, servlen, flags) < 0)
		throw sio::errno_exception("get_nameinfo", sio::socket_errno());
}

SIO_INLINE void sio::get_addrinfo(const char* node, const char* serv,
		const sio::addrinfo_t* hints, sio::addrinfo_t** result)
{
	if (::getaddrinfo(node, serv, hints, result) < 0)
		throw sio::errno_exception("get_addrinfo", sio::socket_errno());
}

SIO_INLINE void sio::free_addrinfo(sio::addrinfo_t* result)
{
	::freeaddrinfo(result);
}
