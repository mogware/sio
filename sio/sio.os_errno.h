#ifndef __SIO_OS_ERRNO__
#define __SIO_OS_ERRNO__

#include "sio.config.h"

#include <string>
#include <exception>

#include "errno.h"

#if !defined(ESOCKTNOSUPPORT)
#define ESOCKTNOSUPPORT 900
#endif
#if !defined(EPFNOSUPPORT)
#define EPFNOSUPPORT 901
#endif
#if !defined(ESHUTDOWN)
#define ESHUTDOWN 902
#endif
#if !defined(ETOOMANYREFS)
#define ETOOMANYREFS 903
#endif
#if !defined(EHOSTDOWN)
#define EHOSTDOWN 904
#endif
#if !defined(EPROCLIM)
#define EPROCLIM 905
#endif
#if !defined(EUSERS)
#define EUSERS 906
#endif
#if !defined(EDQUOT)
#define EDQUOT 907
#endif
#if !defined(ESTALE)
#define ESTALE 907
#endif
#if !defined(EREMOTE)
#define EREMOTE 908
#endif

namespace sio
{
	/**
	* Returns the error status for the last sockets operation that failed.
	*/
	int socket_errno(const int& err = 0);

	/**
	* Returns the calling thread's last error code value.
	*/
	int system_errno(void);

	class errno_exception : public std::exception
	{
		std::string what_;
		int errno_;
	public:
		explicit errno_exception(const std::string& name, const int& err);
	public:
		SIO_INLINE int get_errno(void) const;
		SIO_INLINE const char* what(void) const noexcept;
	};

	bool inprogress(const errno_exception& e);
}

#if defined(__SIO_INLINE__)
#include "sio.os_errno.inl"
#endif

#endif
