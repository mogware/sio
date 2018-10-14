#ifndef __SIO_CONFIG__
#define __SIO_CONFIG__

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && !defined(__CYGWIN__)
#define SIO_WIN_NATIVE
#endif

#if defined(__CYGWIN__)
#define SIO_CYGWIN
#endif

#if defined(SIO_WIN_NATIVE) || defined(SIO_CYGWIN)
#define SIO_WIN32
#endif

#if !defined(SIO_WIN_NATIVE)
#define SIO_POSIX
#endif

#if defined(_MSC_VER)
#define SIO_MSVC
#if !defined(_DEBUG) && !defined(__SIO_INLINE__)
#define __SIO_INLINE__
#endif
#endif

#if defined(__GNUC__)
#define SIO_GCC
#if __GNUC__ < 4
#define SIO_GCC3
#endif
#if !defined (__SIO_INLINE__)
#define __SIO_INLINE__
#endif
#endif

#if defined(__linux)
#define SIO_HAVE_EPOLL
#define SIO_HAVE_POLL
#define SIO_HAVE_POSIX_SELECT
#elif defined(__sun) || defined(__hpux)
#define SIO_HAVE_DEVPOLL
#define SIO_HAVE_POLL
#define SIO_HAVE_POSIX_SELECT
#define SIO_HAVE_SYS_FILIO_H
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#define SIO_HAVE_KQUEUE
#define SIO_HAVE_POLL
#define SIO_HAVE_POSIX_SELECT
#elif defined(__APPLE__)
#define SIO_HAVE_KQUEUE
#define SIO_HAVE_POSIX_SELECT
#elif defined(__CYGWIN__) || defined(__WIN32) || defined(_WIN32) || defined(WIN32)
#define SIO_HAVE_WIN32_SELECT
#define SIO_HAVE_WSAPOLL
#else // generic POSIX
#if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 200100)
#define SIO_HAVE_POLL
#endif
#define SIO_HAVE_POSIX_SELECT
#endif

#if defined(SIO_LACKS_INLINE_FUNCTIONS) && !defined(SIO_NO_INLINE)
#define SIO_NO_INLINE
#endif

#if defined(SIO_NO_INLINE)
#undef __SIO_INLINE__
#endif

#if defined(__SIO_INLINE__)
#define SIO_INLINE inline
#else
#define SIO_INLINE
#endif

#if defined(SIO_HAVE_WIN32_SELECT)
#include <map>
#include <algorithm>
#endif

#if defined(SIO_HAVE_POLL)
#include <poll.h>
#endif

#if defined(SIO_HAVE_DEVPOLL)
#include <sys/devpoll.h>
#include <sys/stat.h>
#endif

#if defined(SIO_HAVE_EPOLL)
#include <sys/epoll.h>
#endif

#if defined(SIO_HAVE_KQUEUE)
#include <sys/event.h>
#endif

#if defined(SIO_WIN32)
#if defined(FD_SETSIZE)
#undef FD_SETSIZE
#endif
#define FD_SETSIZE 1024
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#if !defined(SIO_LACKS_NETINET_IN_H)
#include <netinet/in.h>
#endif
#if !defined(SIO_LACKS_NETINET_TCP_H)
#include <netinet/tcp.h>
#endif
#if !defined(SIO_LACKS_NETDB_H)
#include <netdb.h>
#endif
#if !defined(SIO_LACKS_ARPA_INET_H)
#include <arpa/inet.h>
#endif
#if defined(SIO_HAVE_INETLIB_H)
#include <inetLib.h>
#endif
#if !defined(SIO_LACKS_SYS_TIME_H)
#include <sys/time.h>
#endif
#if !defined(SIO_LACKS_TIME_H)
#include <time.h>
#endif
#if !defined(SIO_LACKS_SYS_TYPES_H)
#include <sys/types.h>
#endif
#if defined(SIO_HAVE_TYPES_H)
#include <types.h>
#endif
#if !defined(SIO_LACKS_FCNTL_H)
#include <fcntl.h>
#endif
#if !defined(SIO_LACKS_SYS_SOCKET_H)
#include <sys/socket.h>
#endif
#if defined(SIO_HAVE_SOCKLIB_H)
#include <sockLib.h>
#endif
#if !defined(SIO_LACKS_SYS_UTSNAME_H)
#include <sys/utsname.h>
#endif
#if !defined(SIO_LACKS_IOCTL_H)
#include <sys/ioctl.h>
#endif
#if defined(SIO_HAVE_SYS_FILIO_H)
#include <sys/filio.h>
#endif
#if !defined(SIO_LACKS_SYS_FCNTL_H)
#include <sys/fcntl.h>
#endif
#if !defined (SIO_LACKS_SYS_UIO_H)
#include <sys/uio.h>
#endif
#if !defined(SIO_LACKS_UNISTD_H)
#include <unistd.h>
#endif
#endif

#endif
