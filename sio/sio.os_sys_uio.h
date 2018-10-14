#ifndef __SIO_OS_SYS_UIO__
#define __SIO_OS_SYS_UIO__

#include "sio.config.h"

#ifdef __cplusplus
extern "C"
{
#endif
#if defined(SIO_WIN32)
struct iovec
{
	unsigned long iov_len;
	void *iov_base;
	operator WSABUF &(void) { return *((WSABUF *) this); }
};
#endif
#ifdef __cplusplus
}
#endif

namespace sio
{
	typedef struct ::iovec iovec_t;
}

#endif
