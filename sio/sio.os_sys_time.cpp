#include "sio.os_sys_time.h"

int sio::gettimeofday(sio::timeval_t* tv, void *)
{
#if defined(SIO_WIN32)
	::FILETIME ft;
	::GetSystemTimeAsFileTime(&ft);
	long long t = (static_cast<long long>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;

#if !defined(_MSC_VER) || _MSC_VER > 1300 // > VC++ 7.0
	t -= 116444736000000000LL;
#else
	t -= 116444736000000000;
#endif

	t /= 10;  // microseconds
	tv->tv_sec = static_cast<long>(t / 1000000UL);
	tv->tv_usec = static_cast<long>(t % 1000000UL);
	return 0;
#else
	return ::gettimeofday(tv, nullptr);
#endif
}
