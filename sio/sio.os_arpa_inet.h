#ifndef __SIO_OS_ARPA_INET__
#define __SIO_OS_ARPA_INET__

#include "sio.config.h"
#include "sio.os_errno.h"

namespace sio
{
	typedef struct ::in_addr in_addr_t;

	/**
	* Converts a string containing an IPv4 dotted-decimal address into a
	* proper address.
	*/
	SIO_INLINE unsigned long inet_addr(const char* name);

	/**
	* Converts a string containing an dotted-decimal address into a
	* proper address in the IN_ADDR structure.
	*/
	SIO_INLINE bool inet_aton(const char* src, in_addr_t* addr);

	/**
	* Converts an (Ipv4) Internet network address into an ASCII string in
	* Internet standard dotted-decimal format.
	*/
	SIO_INLINE const char* inet_ntoa(const in_addr_t addr);

	/**
	*  Converts an IPv4 or IPv6 Internet network address into a string in
	* Internet standard format.
	*/
	SIO_INLINE const char* inet_ntop(const int& af, const void* src,
		char* dst, const int& len);

	/**
	* Converts an IPv4 or IPv6 Internet network address in its standard
	* text presentation form into its numeric binary form.
	*/
	SIO_INLINE void inet_pton(const int& af, const char* src, void* dst);

	/**
	* Converts a unsigned long from TCP/IP network order to host byte order.
	*/
	SIO_INLINE unsigned long ntohl(const unsigned long& n);

	/**
	* Converts a unsigned long from host to TCP/IP network byte order.
	*/
	SIO_INLINE unsigned long htonl(const unsigned long& n);

	/**
	* Converts a unsigned short from TCP/IP network order to host byte order.
	*/
	SIO_INLINE unsigned short ntohs(const unsigned short& n);

	/**
	* Converts a unsigned short from host to TCP/IP network byte order.
	*/
	SIO_INLINE unsigned short htons(const unsigned short& n);
}

#if defined(__SIO_INLINE__)
#include "sio.os_arpa_inet.inl"
#endif

#endif


