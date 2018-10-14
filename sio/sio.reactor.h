#ifndef __SIO_REACTOR__
#define __SIO_REACTOR__

#include <memory>

#include "sio.config.h"
#include "sio.os_errno.h"
#include "sio.os_sys_uio.h"
#include "sio.os_sys_socket.h"

namespace sio
{
	struct reactor_impl;

	class reactor
	{
		std::shared_ptr<reactor_impl> impl_;
	public:
		static const int in;
		static const int out;
		static const int err;
	public:
		static const int use_best;
		static const int use_select;
		static const int use_poll;
		static const int use_epoll;
		static const int use_dev_poll;
		static const int use_kqueue;
	public:
		struct event
		{
			socket_t sockfd;
			int events;
		};
	public:
		reactor(const int& hint = use_best);
		virtual ~reactor() {}
	public:
		SIO_INLINE std::string name(void) const;
		SIO_INLINE void select(const socket_t& sockfd, const int& flags);
		SIO_INLINE void remove(const socket_t& sockfd);
		SIO_INLINE int poll(reactor::event* events,
				const int& n, const int& timeout);
	};

	struct reactor_impl
	{
		virtual std::string name(void) const = 0;
		virtual void select(const socket_t& sockfd, const int& flags) = 0;
		virtual int poll(reactor::event* events,
				const int& n, const int& timeout) = 0;
		virtual ~reactor_impl(void) {};
	};
}

#if defined(__SIO_INLINE__)
#include "sio.reactor.inl"
#endif

#endif
