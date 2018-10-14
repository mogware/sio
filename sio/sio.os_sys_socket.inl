extern int _socket_nread(const sio::socket_t& sockfd);
extern void _socket_nbio(const sio::socket_t& sockfd, const bool& on);

SIO_INLINE void sio::bind(const sio::socket_t& sockfd, const sio::sockaddr_t* addr, const int& len)
{
	if (::bind(sockfd, addr, len) < 0)
		throw sio::errno_exception("bind", sio::socket_errno());
}

SIO_INLINE void sio::listen(const sio::socket_t& sockfd, const int& backlog)
{
	if (::listen(sockfd, backlog) < 0)
		throw sio::errno_exception("listen", sio::socket_errno());
}

SIO_INLINE void sio::shutdown(const sio::socket_t& sockfd, const int& how)
{
	if (::shutdown(sockfd, how) < 0)
		throw sio::errno_exception("shutdown", sio::socket_errno());
}

SIO_INLINE void sio::closesocket(const sio::socket_t& sockfd)
{
#if defined(SIO_WIN32)
	if (::closesocket(sockfd) < 0)
		throw sio::errno_exception("closesocket", sio::socket_errno());
#else
	if (::close(sockfd) < 0)
		throw sio::errno_exception("closesocket", sio::socket_errno());
#endif
}

SIO_INLINE void sio::ioctlsocket(const sio::socket_t& sockfd,
		const sio::ioctlcode_t& code, int* argp)
{
	if (code == sio::sio_nread)
		*argp = _socket_nread(sockfd);
	else if (code == sio::sio_nbio)
		_socket_nbio(sockfd, argp != nullptr && *argp != 0);
	else
		throw sio::errno_exception("ioctlsocket", EINVAL);
}

SIO_INLINE void sio::getpeername(const sio::socket_t& sockfd,
		sio::sockaddr_t* addr, int* len)
{
#if defined(SIO_WIN32)
	if (::getpeername(sockfd, addr, len) < 0)
#else
	if (::getpeername(sockfd, addr, reinterpret_cast<socklen_t*>(len)) < 0)
#endif
		throw sio::errno_exception("getpeername", sio::socket_errno());
}

SIO_INLINE void sio::getsockname(const sio::socket_t& sockfd,
		sio::sockaddr_t* addr, int* len)
{
#if defined(SIO_WIN32)
	if (::getsockname(sockfd, addr, len) < 0)
#else
	if (::getsockname(sockfd, addr, reinterpret_cast<socklen_t*>(len)) < 0)
#endif
		throw sio::errno_exception("getsockname", sio::socket_errno());
}

SIO_INLINE void sio::getsockopt(const sio::socket_t& sockfd, const int& level,
		const int& optname, void* optval, int* optlen)
{
#if defined(SIO_WIN32)
	int result = ::getsockopt(sockfd, level, optname, static_cast<char*>(optval), optlen);
	if (result != 0 && level == IPPROTO_IPV6 && optname == IPV6_V6ONLY
		&& ::WSAGetLastError() == WSAENOPROTOOPT && *optlen == sizeof(DWORD))
	{
		// Dual-stack IPv4/v6 sockets, and the IPV6_V6ONLY socket option, are only
		// supported on Windows Vista and later. To simplify program logic we will
		// fake success of getting this option and specify that the value is
		// non-zero (i.e. true). This corresponds to the behavior of IPv6 sockets
		// on Windows platforms pre-Vista.
		*static_cast<DWORD*>(optval) = 1;
	}
	else if (result != 0)
		throw sio::errno_exception("setsockopt", sio::socket_errno());
#else
	int result = ::getsockopt(sockfd, level, optname, optval, reinterpret_cast<socklen_t*>(optlen));
#if defined(__linux__)
	if (result == 0 && level == SOL_SOCKET && *optlen == sizeof(int)
		&& (optname == SO_SNDBUF || optname == SO_RCVBUF))
	{
		// On Linux, setting SO_SNDBUF or SO_RCVBUF to N actually causes the kernel
		// to set the buffer size to N*2. Linux puts additional stuff into the
		// buffers so that only about half is actually available to the application.
		// The retrieved value is divided by 2 here to make it appear as though the
		// correct value has been set.
		*static_cast<int*>(optval) /= 2;
	}
	else
#endif
	if (result != 0)
		throw sio::errno_exception("setsockopt", sio::socket_errno());
#endif
}

SIO_INLINE void sio::setsockopt(const sio::socket_t& sockfd, const int& level,
		const int& optname, const void* optval, const int& optlen)
{
#if defined(SIO_WIN32)
	if (::setsockopt(sockfd, level, optname, static_cast<const char*>(optval), optlen) < 0)
#else
	if (::setsockopt(sockfd, level, optname, optval, optlen) < 0)
#endif
		throw sio::errno_exception("setsockopt", sio::socket_errno());
}

SIO_INLINE int sio::recv(const sio::socket_t& sockfd, void* buffer,
	const int& bytes, const int& flags)
{
	iovec_t iov[1];
	iov[0].iov_base = buffer;
	iov[0].iov_len = bytes;
	return sio::recvmsg(sockfd, iov, 1, flags);
}

SIO_INLINE int sio::send(const sio::socket_t& sockfd, const void* buffer,
	const int& bytes, const int& flags)
{
	iovec_t iov[1];
	iov[0].iov_base = const_cast<void *>(buffer);
	iov[0].iov_len = bytes;
	return sio::sendmsg(sockfd, iov, 1, flags);
}
