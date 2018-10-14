#include "sio.os_errno.h"

#include <sstream>

#if !defined(__SIO_INLINE__)
#include "sio.os_errno.inl"
#endif

int sio::socket_errno(const int& err)
{
#if defined(SIO_WIN32)
	int error = (err == 0) ? ::WSAGetLastError() : err;
	switch (error) {
	case WSAEWOULDBLOCK:
		return EWOULDBLOCK;
	case WSAEINPROGRESS:
		return EINPROGRESS;
	case WSAEALREADY:
		return EALREADY;
	case WSAENOTSOCK:
		return ENOTSOCK;
	case WSAEDESTADDRREQ:
		return EDESTADDRREQ;
	case WSAEMSGSIZE:
		return EMSGSIZE;
	case WSAEPROTOTYPE:
		return EPROTOTYPE;
	case WSAENOPROTOOPT:
		return ENOPROTOOPT;
	case WSAEPROTONOSUPPORT:
		return EPROTONOSUPPORT;
	case WSAESOCKTNOSUPPORT:
		return ESOCKTNOSUPPORT;
	case WSAEOPNOTSUPP:
		return EOPNOTSUPP;
	case WSAEPFNOSUPPORT:
		return EPFNOSUPPORT;
	case WSAEAFNOSUPPORT:
		return EAFNOSUPPORT;
	case WSAEADDRINUSE:
		return EADDRINUSE;
	case WSAEADDRNOTAVAIL:
		return EADDRNOTAVAIL;
	case WSAENETDOWN:
		return ENETDOWN;
	case WSAENETUNREACH:
		return ENETUNREACH;
	case WSAENETRESET:
		return ENETRESET;
	case WSAECONNABORTED:
		return ECONNABORTED;
	case WSAECONNRESET:
		return ECONNRESET;
	case WSAENOBUFS:
		return ENOBUFS;
	case WSAEISCONN:
		return EISCONN;
	case WSAENOTCONN:
		return ENOTCONN;
	case WSAESHUTDOWN:
		return ESHUTDOWN;
	case WSAETOOMANYREFS:
		return ETOOMANYREFS;
	case WSAETIMEDOUT:
		return ETIMEDOUT;
	case WSAECONNREFUSED:
		return ECONNREFUSED;
	case WSAELOOP:
		return ELOOP;
	case WSAENAMETOOLONG:
		return ENAMETOOLONG;
	case WSAEHOSTDOWN:
		return EHOSTDOWN;
	case WSAEHOSTUNREACH:
		return EHOSTUNREACH;
	case WSAENOTEMPTY:
		return ENOTEMPTY;
	case WSAEPROCLIM:
		return EPROCLIM;
	case WSAEUSERS:
		return EUSERS;
	case WSAEDQUOT:
		return EDQUOT;
	case WSAESTALE:
		return ESTALE;
	case WSAEREMOTE:
		return EREMOTE;
	default:
		return error;
	}
#else
	return (err == 0) ? errno : err;
#endif
}

int sio::system_errno(void)
{
#if defined(SIO_WIN32)
	return ::GetLastError();
#else
	return errno;
#endif
}

sio::errno_exception::errno_exception(const std::string& name, const int& err)
	: what_("")
	, errno_(err)
{
	std::ostringstream stream;
	stream << err;
	what_ = name + " failed: errno " + stream.str();
}

bool sio::inprogress(const sio::errno_exception& e)
{
#if defined(SIO_WIN32)
	return e.get_errno() == EWOULDBLOCK;
#else
	return e.get_errno() == EINPROGRESS;
#endif
}