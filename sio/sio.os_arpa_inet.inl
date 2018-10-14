SIO_INLINE unsigned long sio::inet_addr(const char* name)
{
#if defined(SIO_WIN32)
	sio::in_addr_t addr;
	sio::inet_pton(AF_INET, name, &addr);
	return addr.s_addr;
#elif defined(SIO_HAS_NONCONST_INET_ADDR)
	return ::inet_addr(const_cast<char*>(name));
#else
	return ::inet_addr(name);
#endif
}

SIO_INLINE bool sio::inet_aton(const char* src, sio::in_addr_t* addr)
{
#if defined(SIO_WIN32)
	return ::inet_pton(AF_INET, src, addr) != -1;
#else
	return ::inet_aton(src, addr) != 0;
#endif
}

SIO_INLINE const char* sio::inet_ntoa(const sio::in_addr_t addr)
{
#if defined(SIO_WIN32)
	char str[INET_ADDRSTRLEN];
	return sio::inet_ntop(AF_INET, &addr, str, sizeof(str));
#else
	return ::inet_ntoa(addr);
#endif
}

SIO_INLINE const char* sio::inet_ntop(const int& af, const void* src,
	char* dst, const int& len)
{
	const char* addr = ::inet_ntop(af, (void*)src, dst, len);
	if (addr == nullptr)
		throw sio::errno_exception("inet_ntop", sio::socket_errno());
	return addr;

}

SIO_INLINE void sio::inet_pton(const int& af, const char* src, void* dst)
{
	if (::inet_pton(af, src, dst) < 0)
		throw sio::errno_exception("inet_pton", sio::socket_errno());
}

SIO_INLINE unsigned long sio::ntohl(const unsigned long& n)
{
	return ::ntohl(n);
}

SIO_INLINE unsigned long sio::htonl(const unsigned long& n)
{
	return ::htonl(n);
}

SIO_INLINE unsigned short sio::ntohs(const unsigned short& n)
{
	return ::ntohs(n);
}

SIO_INLINE unsigned short sio::htons(const unsigned short& n)
{
	return ::htons(n);
}
