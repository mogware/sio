#ifndef __SIO_OS_SYS_SOCKET__
#define __SIO_OS_SYS_SOCKET__

#include "sio.config.h"

#if defined(SD_RECEIVE)
#define SIO_SHUTDOWN_READ SD_RECEIVE
#elif defined(SHUT_RD)
#define SIO_SHUTDOWN_READ SHUT_RD
#else
#define SIO_SHUTDOWN_READ 0
#endif

#if defined(SD_SEND)
#define SIO_SHUTDOWN_WRITE SD_SEND
#elif defined(SHUT_WR)
#define SIO_SHUTDOWN_WRITE SHUT_WR
#else
#define SIO_SHUTDOWN_WRITE 1
#endif

#if defined(SD_BOTH)
#define SIO_SHUTDOWN_BOTH SD_BOTH
#elif defined(SHUT_RDWR)
#define SIO_SHUTDOWN_BOTH SHUT_RDWR
#else
#define SIO_SHUTDOWN_BOTH 2
#endif

#if !defined(AF_UNIX)
#define AF_UNIX AF_INET
#endif
#if !defined(AF_LOCAL)
#define AF_LOCAL AF_UNIX
#endif
#if !defined(PF_LOCAL)
#define PF_LOCAL PF_INET
#endif

namespace sio
{
#if defined (SIO_WIN32)
	extern int socket_initialized_;

	typedef ::SOCKET socket_t;
	typedef struct ::fd_set fdset_t;
#else
	typedef int socket_t;
	typedef ::fd_set fdset_t;
#endif
	extern const socket_t invalid_socket;

	typedef struct ::sockaddr sockaddr_t;
	typedef struct ::sockaddr_in sockaddr_in_t;
	typedef struct ::sockaddr_in6 sockaddr_in6_t;
	typedef struct ::addrinfo addrinfo_t;

	typedef enum { sio_nread, sio_nbio } ioctlcode_t;

	/**
	* Initiates use of sockets by a process.
	*/
	void socket_init(const int& version_high = 1, const int& version_low = 1);

	/**
	* Creates a socket that is bound to a specific transport-service provider.
	*/
	socket_t socket(const int& family, const int& type, const int& protocol);

	/**
	* Creates an unnamed pair of connected sockets.
	*/
	void socketpair(const int& family, const int& type, const int& protocol, socket_t sv[2]);

	/**
	* Permits an incoming connection attempt on a socket.
	*/
	socket_t accept(const socket_t& sockfd, sockaddr_t* addr, int* len);

	/**
	* Establishes a connection to a specified socket.
	*/
	void connect(const socket_t& sockfd, const sockaddr_t* addr, const int& len);

	/**
	* Associates a local address with a socket.
	*/
	SIO_INLINE void bind(const socket_t& sockfd, const sockaddr_t* addr, const int& len);

	/**
	* Places a socket a state where it is listening for an incoming connection.
	*/
	SIO_INLINE void listen(const socket_t& sockfd, const int& backlog);

	/**
	* Disables sends or receives on a socket.
	*/
	SIO_INLINE void shutdown(const socket_t& sockfd, const int& how);

	/**
	* Closes an existing socket.
	*/
	SIO_INLINE void closesocket(const socket_t& sockfd);

	/**
	* Controls the I/O mode of a socket.
	*/
	SIO_INLINE void ioctlsocket(const socket_t& sockfd, const ioctlcode_t& code, int* argp);

	/**
	* Retrieves the address of the peer to which a socket is connected.
	*/
	SIO_INLINE void getpeername(const socket_t& sockfd, sockaddr_t* addr, int* len);

	/**
	* Retrieves the local name for a socket.
	*/
	SIO_INLINE void getsockname(const socket_t& sockfd, sockaddr_t* addr, int* len);

	/**
	* Retrieves a socket option.
	*/
	SIO_INLINE void getsockopt(const socket_t& sockfd, const int& level,
		const int& optname, void* optval, int* optlen);

	/**
	* Sets a socket option.
	*/
	SIO_INLINE void setsockopt(const socket_t& sockfd, const int& level,
		const int& optname, const void* optval, const int& optlen);

	/**
	* Receives a message from a connected or connectionless socket.
	*/
	int recvmsg(const socket_t& sockfd, iovec_t* iov, const int& count, const int& flags);

	/**
	* Sends a message to a connected or connectionless socket.
	*/
	int sendmsg(const socket_t& sockfd, const iovec_t* iov, const int& count, const int& flags);

	/**
	* Receives a datagram and stores the source address.
	*/
	int recvfrom(const socket_t& sockfd, iovec_t* iov,
		const int& count, const int& flags, sockaddr_t* addr, int* len);

	/**
	* Sends a datagram to a specific destination.
	*/
	int sendto(const socket_t& sockfd, const iovec_t* iov,
		const int& count, const int& flags, const sockaddr_t* addr, const int& len);

	/**
	* Receives data from a connected or bound socket.
	*/
	SIO_INLINE int recv(const socket_t& sockfd, void* buffer,
		const int& bytes, const int& flags);

	/**
	* Sends data on a connected socket.
	*/
	SIO_INLINE int send(const socket_t& sockfd, const void* buffer,
		const int& bytes, const int& flags);

	/**
	* Terminates use of sockets
	*/
	void socket_term(void);
}

#if defined(__SIO_INLINE__)
#include "sio.os_sys_socket.inl"
#endif

#endif
