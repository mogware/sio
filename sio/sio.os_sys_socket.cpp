#include "sio.os_errno.h"
#include "sio.os_sys_uio.h"
#include "sio.sock_addr4.h"
#include "sio.os_sys_socket.h"

#if defined(SIO_WIN32)
const sio::socket_t sio::invalid_socket = static_cast<sio::socket_t>(~0);
int sio::socket_initialized_ = 0;
#else
const sio::socket_t sio::invalid_socket = -1;
#endif

#if !defined(__SIO_INLINE__)
#include "sio.os_sys_socket.inl"
#endif

void sio::socket_init(const int& version_high, const int& version_low)
{
#if defined(SIO_WIN32) && !defined(SIO_DONT_INIT_WINSOCK)
	if (sio::socket_initialized_ == 0)
	{
		WORD version_requested = MAKEWORD(version_high, version_low);
		WSADATA wsa_data;
		int error = ::WSAStartup(version_requested, &wsa_data);
		if (error != 0)
			throw sio::errno_exception("socket_init", error);
		sio::socket_initialized_ = 1;
	}
#endif
}

sio::socket_t sio::socket(const int& family, const int& type, const int& protocol)
{
	sio::socket_t sock = sio::invalid_socket;
#if defined(SIO_WIN32)
	sock = ::WSASocket(family, type, protocol, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (sock == sio::invalid_socket)
		throw sio::errno_exception("socket", sio::socket_errno());
	if (family == AF_INET6)
	{
		// Try to enable the POSIX default behaviour of having IPV6_V6ONLY set to false
		DWORD optval = 0;
		::setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY,
			reinterpret_cast<const char*>(&optval), sizeof(optval));
	}
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__)
	sock = ::socket(family, type, protocol);
	if (sock == sio::invalid_socket)
		throw sio::errno_exception("socket", sio::socket_errno());
	int optval = 1;
	int result = ::setsockopt(sock,
		SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
	if (result != 0)
	{
		close(sock)
		throw sio::errno_exception("socket", sio::socket_errno());
	}
#else
	sock = ::socket(family, type, protocol);
	if (sock == sio::invalid_socket)
		throw sio::errno_exception("socket", sio::socket_errno());
#endif
	return sock;
}

void sio::socketpair(const int& family, const int& type,
		const int& protocol, sio::socket_t sv[2])
{
#if defined(SIO_WIN32)
	if (family != AF_LOCAL && family != AF_INET)
		throw sio::errno_exception("socketpair", EAFNOSUPPORT);
	if (type != SOCK_STREAM && type != SOCK_DGRAM)
		throw sio::errno_exception("socketpair", EOPNOTSUPP);
	if ((family == AF_LOCAL && protocol != PF_UNSPEC && protocol != PF_LOCAL)
			|| (family == AF_INET && protocol != PF_UNSPEC && protocol != PF_INET))
		throw sio::errno_exception("socketpair", EPROTONOSUPPORT);
	sio::socket_t sock = sio::invalid_socket;
	sio::socket_t insock = sio::invalid_socket;
	sio::socket_t outsock = sio::invalid_socket;
	sio::sock_addr4 sock_in(INADDR_LOOPBACK, 0);
	sio::sock_addr4 sock_out(INADDR_LOOPBACK, 0);
	try {
		sock = sio::socket(AF_INET, type, 0);
		int n = 1;
		sio::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
		sio::bind(sock, sock_in, sizeof(sock_in));
		n = sizeof(sock_in);
		sio::getsockname(sock, sock_in, &n);
		if (type == SOCK_STREAM)
			sio::listen(sock, 2);
		outsock = sio::socket(AF_INET, type, 0);
		if (type == SOCK_DGRAM)
		{
			sio::bind(outsock, sock_out, sizeof(sock_out));
			n = sizeof(sock_out);
			sio::getsockname(outsock, sock_out, &n);
		}
		sio::connect(outsock, sock_in, sizeof(sock_in));
		if (type == SOCK_STREAM)
		{
			n = sizeof(sock_in);
			insock = sio::accept(sock, sock_in, &n);
			closesocket(sock);
			sock = sio::invalid_socket;
		}
		else
		{
			sio::connect(sock, sock_out, sizeof(sock_out));
			insock = sock;
			sock = sio::invalid_socket;
		}
		sv[0] = insock;
		sv[1] = outsock;
	}
	catch (const sio::errno_exception& e)
	{
		if (sock != sio::invalid_socket)
			sio::closesocket(sock);
		if (insock != sio::invalid_socket)
			sio::closesocket(insock);
		if (outsock != sio::invalid_socket)
			sio::closesocket(outsock);
		throw e;
	}
#else
	if (::socketpair(family, type, protocol, sv) < 0)
		throw sio::errno_exception("socketpair", sio::socket_errno());
#endif
}
void sio::connect(const sio::socket_t& sockfd, const sio::sockaddr_t* addr, const int& len)
{
	int result = -1;
#if defined(SIO_WIN32)
	result = ::WSAConnect(sockfd, addr, len, nullptr, nullptr, nullptr, nullptr);
#else
	result = ::connect(sockfd, addr, len);
#endif
	if (result != 0)
		throw sio::errno_exception("connect", sio::socket_errno());
}

sio::socket_t sio::accept(const sio::socket_t& sockfd, sio::sockaddr_t* addr, int* len)
{
	sio::socket_t sock = sio::invalid_socket;
#if defined(SIO_WIN32)
	sock = ::WSAAccept(sockfd, addr, len, nullptr, 0);
	if (sock == sio::invalid_socket)
		throw sio::errno_exception("socket", sio::socket_errno());
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__)
	sock = ::accept(sockfd, addr, reinterpret_cast<socklen_t*>(len));
	if (sock == sio::invalid_socket)
		throw sio::errno_exception("socket", sio::socket_errno());
	int optval = 1;
	int result = ::setsockopt(sock,
		SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
	if (result != 0)
	{
		close(sock)
		throw sio::errno_exception("socket", sio::socket_errno());
	}
#else
	sock = ::accept(sockfd, addr, reinterpret_cast<socklen_t*>(len));
	if (sock == sio::invalid_socket)
		throw sio::errno_exception("accept", sio::socket_errno());
#endif
	return sock;
}

int _socket_nread(const sio::socket_t& sockfd)
{
#if defined(SIO_WIN32)
	unsigned long n = 0;
	if (::ioctlsocket(sockfd, FIONREAD, &n) < 0)
		throw sio::errno_exception("ioctlsocket", sio::socket_errno());
	return n;
#else
	int n = 0;
	if (::ioctl(sockfd, FIONREAD, &n) < 0)
		throw sio::errno_exception("ioctlsocket", sio::socket_errno());
	return n;
#endif
	return 0;
}

void _socket_nbio(const sio::socket_t& sockfd, const bool& on)
{
#if defined(SIO_WIN32)
	unsigned long n = on ? 1 : 0;
	if (::ioctlsocket(sockfd, FIONBIO, &n) < 0)
		throw sio::errno_exception("ioctlsocket", sio::socket_errno());
#elif defined(SIO_LACKS_FCNTL)
	int n = on ? 1 : 0;
	if (::ioctl(sockfd, FIONBIO, &n) < 0)
		throw sio::errno_exception("ioctlsocket", sio::socket_errno());
#else
	int flags = 0;
	if ((flags = ::fcntl(sockfd, F_GETFL, NULL)) < 0)
		throw sio::errno_exception("ioctlsocket", sio::socket_errno());
	if (on)
	{
		if (flags & O_NONBLOCK)
			return;
		flags |= O_NONBLOCK;
	}
	else
	{
		if (!(flags & O_NONBLOCK))
			return;
		flags &= ~O_NONBLOCK;
	}
	if (::fcntl(sockfd, F_SETFL, flags) < 0)
		throw sio::errno_exception("ioctlsocket", sio::socket_errno());
#endif
}

int sio::recvmsg(const sio::socket_t& sockfd, sio::iovec_t* iov,
		const int& count, const int& flags)
{
#if defined(SIO_WIN32)
	DWORD bytes_received = 0;
	DWORD recv_flags = flags;
	int result = ::WSARecv(sockfd, reinterpret_cast<::WSABUF *>(iov),
		count, &bytes_received, &recv_flags, 0, 0);
	if (result != 0)
		throw sio::errno_exception("recvmsg", sio::socket_errno());
	return bytes_received;
#else
	msghdr msg = msghdr();
	msg.msg_iov = iov;
	msg.msg_iovlen = count;
	int nbytes = ::recvmsg(sockfd, &msg, flags);
	if (nbytes < 0)
		throw sio::errno_exception("recvmsg", sio::socket_errno());
	return nbytes;
#endif
}

int sio::recvfrom(const sio::socket_t& sockfd, sio::iovec_t* iov,
	const int& count, const int& flags, sockaddr_t* addr, int* len)
{
#if defined(SIO_WIN32)
	DWORD bytes_received = 0;
	DWORD recv_flags = flags;
	int result = ::WSARecvFrom(sockfd, reinterpret_cast<::WSABUF *>(iov),
		count, &bytes_received, &recv_flags, addr, len, 0, 0);
	if (result != 0)
		throw sio::errno_exception("recvfrom", sio::socket_errno());
	return bytes_received;
#else
	msghdr msg = msghdr();
	msg.msg_name = addr;
	msg.msg_namelen = *len;
	msg.msg_iov = iov;
	msg.msg_iovlen = count;
	int nbytes = ::recvmsg(sockfd, &msg, flags);
	if (nbytes < 0)
		throw sio::errno_exception("recvfrom", sio::socket_errno());
	return nbytes;
#endif
}

int sio::sendmsg(const sio::socket_t& sockfd, const sio::iovec_t* iov,
		const int& count, const int& flags)
{
#if defined(SIO_WIN32)
	DWORD bytes_sent = 0;
	int result = ::WSASend(sockfd,
		const_cast<::WSABUF *>(reinterpret_cast<const ::WSABUF *>(iov)),
		count, &bytes_sent, 0, 0, 0);
	if (result != 0)
		throw sio::errno_exception("sendv", sio::socket_errno());
	return bytes_sent;
#else
	msghdr msg = msghdr();
	msg.msg_iov = const_cast<iovec_t *>(iov);
	msg.msg_iovlen = count;
	int nbytes = 0;
#if defined(__linux__)
	nbytes = ::sendmsg(sockfd, &msg, flags | MSG_NOSIGNAL);
#else
	nbytes = ::sendmsg(sockfd, &msg, flags);
#endif
	if (nbytes < 0)
		throw sio::errno_exception("sendv", sio::socket_errno());
	return nbytes;
#endif
}

int sio::sendto(const sio::socket_t& sockfd, const sio::iovec_t* iov,
	const int& count, const int& flags, const sockaddr_t* addr, const int& len)
{
#if defined(SIO_WIN32)
	DWORD bytes_sent = 0;
	int result = ::WSASendTo(sockfd,
		const_cast<::WSABUF *>(reinterpret_cast<const ::WSABUF *>(iov)),
		count, &bytes_sent, flags, addr, len, 0, 0);
	if (result != 0)
		throw sio::errno_exception("sendv", sio::socket_errno());
	return bytes_sent;
#else
	msghdr msg = msghdr();
	msg.msg_name = const_cast<sockaddr_t *>(addr);
	msg.msg_namelen = len;
	msg.msg_iov = const_cast<iovec_t *>(iov);
	msg.msg_iovlen = count;
	int nbytes = 0;
#if defined(__linux__)
	nbytes = ::sendmsg(sockfd, &msg, flags | MSG_NOSIGNAL);
#else
	nbytes = ::sendmsg(sockfd, &msg, flags);
#endif
	if (nbytes < 0)
		throw sio::errno_exception("sendv", sio::socket_errno());
	return nbytes;
#endif
}

void sio::socket_term(void)
{
#if defined(SIO_WIN32)
	if (sio::socket_initialized_ != 0)
	{
		if (::WSACleanup() < 0)
			throw sio::errno_exception("socket_term", sio::socket_errno());
		sio::socket_initialized_ = 0;
	}
#endif
}
