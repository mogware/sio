SIO_INLINE sio::addrinfo::addrinfo(void)
{
	std::memset(this, 0, sizeof(addrinfo_t));
	ai_family = AF_UNSPEC;
	ai_socktype = SOCK_STREAM;
}

SIO_INLINE sio::addrinfo::operator sio::addrinfo_t* ()
{
	return reinterpret_cast<addrinfo_t*>(this);
}
