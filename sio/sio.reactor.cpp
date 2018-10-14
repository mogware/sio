#include "sio.reactor.h"
#include "sio.reactor_impl.h"

const int sio::reactor::in = 1 << 0;
const int sio::reactor::out = 1 << 1;
const int sio::reactor::err = 1 << 2;

const int sio::reactor::use_best = 0;
const int sio::reactor::use_select = 1;
const int sio::reactor::use_poll = 2;
const int sio::reactor::use_epoll = 3;
const int sio::reactor::use_dev_poll = 4;
const int sio::reactor::use_kqueue = 5;

#if !defined(__SIO_INLINE__)
#include "sio.reactor.inl"
#endif

sio::reactor::reactor(const int& hint)
	: impl_(nullptr)
{
	static const int use_best_poll =
#if defined(SIO_HAVE_EPOLL)
		use_epoll;
#elif defined(SIO_HAVE_DEVPOLL)
		use_dev_poll;
#elif defined(SIO_HAVE_KQUEUE)
		use_kqueue;
#elif defined(SIO_HAVE_POLL) || defined(SIO_HAVE_WSAPOLL)
		use_poll;
#else
		use_select;
#endif
	int which = (hint == use_best) ? use_best_poll : hint;
	for (;;)
	{
		switch (which)
		{
#if defined(SIO_HAVE_POSIX_SELECT)
		case use_select:
			impl_ = select_reactor::create();
			return;
#elif defined(SIO_HAVE_WIN32_SELECT)
		case use_select:
			impl_ = win32_select_reactor::create();
			return;
#endif
#if defined(SIO_HAVE_POLL) || defined(SIO_HAVE_WSAPOLL)
		case use_poll:
			impl_ = poll_reactor::create();
			return;
#endif
#if defined(SIO_HAVE_EPOLL)
		case use_epoll:
			impl_ = epoll_reactor::create();
			return;
#endif
#if defined(SIO_HAVE_DEVPOLL)
		case use_dev_poll:
			impl_ = devpoll_reactor::create();
			return;
#endif
#if defined(SIO_HAVE_KQUEUE)
		case use_kqueue:
			impl_ = kqueue_reactor::create();
			return;
#endif
		default:
			if (which != use_best_poll)
				which = use_best_poll;
			else
				throw std::runtime_error("Internal error - no poller found");
		}
	}
}
