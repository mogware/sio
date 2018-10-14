#include "sio.os_errno.h"
#include "sio.os_sys_utsname.h"

std::string sio::uname_sysname(void)
{
#if defined(SIO_WIN32)
	return "Win32";
#elif !defined(SIO_LACKS_UNAME)
	struct utsname utsname;
	if (::uname(&utsname) >= 0)
		return utsname.sysname;
#else
	return "unknown";
#endif
	throw sio::errno_exception("uname_sysname", sio::system_errno());
}

std::string sio::uname_nodename(void)
{
#if defined(SIO_WIN32)
	char nodename[1025];
	DWORD size = sizeof(nodename) - 1;
	if (::GetComputerNameExA(ComputerNameDnsFullyQualified, nodename, &size))
		return nodename;
#elif !defined(SIO_LACKS_UNAME)
	struct utsname utsname;
	if (::uname(&utsname) >= 0)
		return utsname.nodename;
#else
	return "unknown";
#endif
	throw sio::errno_exception("uname_nodename", sio::system_errno());
}
