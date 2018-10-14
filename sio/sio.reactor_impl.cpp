#include "sio.os_errno.h"
#include "sio.os_sys_time.h"
#include "sio.reactor_impl.h"

#include <cstring>

#if !defined(__SIO_INLINE__)
#include "sio.reactor_impl.inl"
#endif

#if defined(SIO_HAVE_POLL) || defined(SIO_HAVE_WSAPOLL) || defined(SIO_HAVE_DEVPOLL)
int sio::base_poll_reactor::to_poll_events(const int& event)
{
	int pe = 0;
	if (event & reactor::in)
		pe |= POLLIN;
	if (event & reactor::out)
		pe |= POLLOUT;
	return pe;
}

int sio::base_poll_reactor::to_user_events(const int& event)
{
	int ue = 0;
	if (event & POLLIN)
		ue |= reactor::in;
	if (event & POLLOUT)
		ue |= reactor::out;
	if (event & (POLLERR | POLLHUP | POLLPRI))
		ue |= reactor::err;
	return ue;
}
#endif

#if defined(SIO_HAVE_POLL) || defined(SIO_HAVE_WSAPOLL)
sio::poll_reactor::poll_reactor(void)
{
}

std::shared_ptr<sio::reactor_impl> sio::poll_reactor::create(void)
{
	return std::make_shared<poll_reactor>();
}

void sio::poll_reactor::select(const sio::socket_t& sockfd, const int& flags)
{
	if (sockfd < 0)
		throw errno_exception("select", EBADF);
	if (flags == 0)
		remove(sockfd);
	else
		entry(sockfd).events = to_poll_events(flags);
}

int sio::poll_reactor::poll(sio::reactor::event* events,
		const int& n, const int& timeout)
{
	struct ::pollfd* pfd = pollfds_.size() > 0 ? &pollfds_.front() : 0;
	std::size_t size = pollfds_.size();

	int count =
#if defined(SIO_HAVE_WSAPOLL)		
		::WSAPoll(pfd, static_cast<std::uint32_t>(size), timeout);
#else
		::poll(pfd, size, timeout);
#endif
	if (count < 0)
		throw errno_exception("poll", sio::socket_errno());

	int read = 0;
	for (unsigned i = 0; i < pollfds_.size() && read < count && read < n; i++) {
		if (pollfds_[i].revents == POLLNVAL) {
			remove(pollfds_[i].fd);
			count--;
		}
		else if (pollfds_[i].revents != 0) {
			events[read].events = to_user_events(pollfds_[i].revents);
			events[read].sockfd = pollfds_[i].fd;
			read++;
		}
	}

	return read;
}

struct pollfd& sio::poll_reactor::entry(const socket_t& sockfd)
{
	if (map_.size() <= unsigned(sockfd))
		map_.resize(sockfd + 1, -1);
	if (map_[sockfd] == -1) {
		map_[sockfd] = pollfds_.size();
		pollfd tmp = pollfd();
		tmp.fd = sockfd;
		pollfds_.push_back(tmp);
	}
	return pollfds_[map_[sockfd]];
}

void sio::poll_reactor::remove(const socket_t& sockfd)
{
	if (sockfd >= int(map_.size()) || map_[sockfd] == -1)
		return;
	std::size_t index = map_[sockfd];
	std::swap(pollfds_[index], pollfds_.back());
	map_[pollfds_[index].fd] = index;
	pollfds_.resize(pollfds_.size() - 1);
	map_[sockfd] = -1;
}
#endif

#if defined(SIO_HAVE_DEVPOLL) || defined(SIO_HAVE_EPOLL) || defined(SIO_HAVE_KQUEUE)
void sio::base_fast_reactor::check(const socket_t& sockfd)
{
	if (sockfd == invalid_socket)
		throw errno_exception("check", EBADF);
	if (int(sockfd) >= int(events_.size()))
		events_.resize(int(sockfd) + 1, 0);
}
#endif

#if defined(SIO_HAVE_DEVPOLL)
sio::devpoll_reactor::devpoll_reactor(void)
{
	pollfd_ = ::open("/dev/poll", O_RDWR);
	if (pollfd_ < 0)
		throw errno_exception("devpoll_reactor", sio::system_errno());
}

sio::devpoll_reactor::~devpoll_reactor(void)
{
	::close(pollfd_);
}

std::shared_ptr<sio::reactor_impl> sio::devpoll_reactor::create(void)
{
	return std::make_shared<devpoll_reactor>();
}

void sio::devpoll_reactor::select(const sio::socket_t& sockfd, const int& flags)
{
	check(sockfd);
	if (flags == events_[sockfd])
		return;

	struct ::pollfd evs[2] = { 0 };
	int count = 0;

	if (events_[sockfd]) {
		evs[count].fd = sockfd;
		evs[count].events = POLLREMOVE;
		count++;

	}

	if (flags != 0) {
		evs[count].fd = sockfd;
		evs[count].events = to_poll_events(flags);
		count++;

	}

	if (count > 0) {
		int size = sizeof(evs[0]) * count;
		if (::write(pollfd_, evs, size) != size)
			throw errno_exception("select", sio::system_errno());
	}

	events_[sockfd] = flags;
}

int sio::devpoll_reactor::poll(	sio::reactor::event* events,
		const int& n, const int& timeout)
{
	struct ::pollfd fds[128] = { 0 };
	struct ::dvpoll dvp = { 0 };
	dvp.dp_timeout = timeout;
	dvp.dp_nfds = n > 128 ? 128 : n;
	dvp.dp_fds = fds;

	int size = ::ioctl(pollfd_, DP_POLL, (u_long*)&dvp);
	if (size < 0)
		throw errno_exception("select", sio::system_errno());

	int read = 0;
	for (int i = 0; i < size; i++) {
		events[read].events = to_user_events(fds[i].revents);
		events[read].sockfd = fds[i].fd;
		read++;

	}

	return read;
}
#endif

#if defined(SIO_HAVE_EPOLL)
sio::epoll_reactor::epoll_reactor(void)
{
	pollfd_ = ::epoll_create(16);
	if (pollfd_ < 0)
		throw errno_exception("epoll_reactor", system_errno());

}

sio::epoll_reactor::~epoll_reactor(void)
{
	::close(pollfd_);
}

std::shared_ptr<sio::reactor_impl> sio::epoll_reactor::create(void)
{
	return std::make_shared<epoll_reactor>();
}

void sio::epoll_reactor::select(const sio::socket_t& sockfd, const int& flags)
{
	check(sockfd);
	if (events_[sockfd] != 0 && flags == 0)
		write_flag(sockfd, EPOLL_CTL_DEL, 0);
	else if (events_[sockfd] == 0 && flags != 0)
		write_flag(sockfd, EPOLL_CTL_ADD, to_poll_events(flags));
	else if (events_[sockfd] != flags)
		write_flag(sockfd, EPOLL_CTL_MOD, to_poll_events(flags));
	events_[sockfd] = flags;
}

int sio::epoll_reactor::poll(sio::reactor::event* events,
		const int& n, const int& timeout)
{
	struct ::epoll_event fds[128] = { 0 };
	int count = (n > 128) ? 128 : n;

	int size = ::epoll_wait(pollfd_, fds, count, timeout);
	if (size < 0)
		throw errno_exception("poll", sio::system_errno());

	int read = 0;
	for (int i = 0; i < size; i++) {
		events[read].events = to_user_events(fds[i].events);
		events[read].sockfd = fds[i].data.fd;
		read++;
	}
	return read;
}

int sio::epoll_reactor::to_poll_events(const int& event)
{
	int pe = 0;
	if (event & reactor::in)
		pe |= EPOLLIN;
	if (event & reactor::out)
		pe |= EPOLLOUT;
	return pe;
}

int sio::epoll_reactor::to_user_events(const int& event)
{
	int ue = 0;
	if (event & EPOLLIN)
		ue |= reactor::in;
	if (event & EPOLLOUT)
		ue |= reactor::out;
	if (event & (EPOLLERR | EPOLLPRI | EPOLLHUP))
		ue |= reactor::err;
	return ue;
}

void sio::epoll_reactor::write_flag(const sio::socket_t& sockfd,
		const int& op, const int& flags)
{
	struct ::epoll_event efd = ::epoll_event();
	efd.events = flags;
	efd.data.fd = sockfd;

	if (::epoll_ctl(pollfd_, op, sockfd, &efd) < 0)
		throw errno_exception("write_flag", sio::system_errno());
}
#endif

#if defined(SIO_HAVE_POSIX_SELECT)
sio::select_reactor::select_reactor(void)
	: map_(FD_SETSIZE, -1)
{
}

std::shared_ptr<sio::reactor_impl> sio::select_reactor::create(void)
{
	return std::make_shared<select_reactor>();
}

void sio::select_reactor::select(const sio::socket_t& sockfd, const int& flags)
{
	if (sockfd == invalid_socket)
		throw errno_exception("check", EBADF);
	if (int(sockfd) >= FD_SETSIZE)
		throw errno_exception("check", EBADF);
	if (flags == 0)
		remove(sockfd);
	else
		entry(sockfd).events = flags;
}

int sio::select_reactor::poll(sio::reactor::event* events,
		const int& n, const int& timeout)
{
	fdset_t rd, wr, er;
	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_ZERO(&er);
	int count = 0;

	for (unsigned i = 0; i < pollfds_.size(); i++) {
		int flags = pollfds_[i].events;
		int sockfd = pollfds_[i].sockfd;
		if (flags & reactor::in)
			FD_SET(sockfd, &rd);
		if (flags & reactor::out)
			FD_SET(sockfd, &wr);
		FD_SET(sockfd, &er);
		if (int(sockfd) + 1> count)
			count = int(sockfd) + 1;
	}

	timeval_t tv, *ptv = nullptr;
	if (timeout >= 0) {
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		ptv = &tv;
	}

	int read = ::select(count, &rd, &wr, &er, ptv);
	if (read < 0)
		throw errno_exception("poll", sio::socket_errno());

	count = 0;

	for (unsigned i = 0; read > 0 && i < pollfds_.size(); i++) {
		int sockfd = pollfds_[i].sockfd;
		bool r = FD_ISSET(sockfd, &rd);
		read -= int(r);
		bool w = FD_ISSET(sockfd, &wr);
		read -= int(w);
		bool e = FD_ISSET(sockfd, &er);
		read -= int(e);
		if (r || w || e) {
			if (count >= n) {
				count++;
				continue;
			}
			reactor::event &ev = events[count];
			ev.sockfd = sockfd;
			ev.events = 0;
			if (r)
				ev.events |= reactor::in;
			if (w)
				ev.events |= reactor::out;
			if (e)
				ev.events |= reactor::err;
			count++;
		}
	}

	return count;
}

sio::select_reactor::event& sio::select_reactor::entry(const sio::socket_t& sockfd)
{
	return pollfds_[map_[sockfd]];
}

void sio::select_reactor::remove(const sio::socket_t& sockfd)
{
	if (int(sockfd) >= int(map_.size()) || map_[sockfd] == -1)
		return;
	int index = map_[sockfd];
	std::swap(pollfds_[index], pollfds_.back());
	map_[pollfds_[index].sockfd] = index;
	pollfds_.resize(pollfds_.size() - 1);
	map_[sockfd] = -1;
}
#endif

#if defined(SIO_HAVE_KQUEUE)
sio::kqueue_reactor::kqueue_reactor(void)
{
	kev_ = ::kqueue();
	if (kev_ < 0)
		throw errno_exception("epoll_reactor", sio::system_errno());
}

sio::kqueue_reactor::~kqueue_reactor(void)
{
	::close(kev_);
}

std::shared_ptr<sio::reactor_impl> sio::kqueue_reactor::create(void)
{
	return std::make_shared<kqueue_reactor>();
}

void sio::kqueue_reactor::select(const sio::socket_t& sockfd, const int& flags)
{
	check(sockfd);

	struct ::kevent evs[2] = { 0 };
	int count = 0;

	if ((events_[sockfd] & reactor::in) && !(flags & reactor::in)) {
		EV_SET(&evs[count], sockfd, EVFILT_READ, EV_DELETE, 0, 0, 0);
		count++;
	}
	if ((events_[sockfd] & reactor::out) && !(flags & reactor::out)) {
		EV_SET(&evs[count], sockfd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
		count++;
	}
	if (!(events_[sockfd] & reactor::in) && (flags & reactor::in)) {
		EV_SET(&evs[count], sockfd, EVFILT_READ, EV_ADD, 0, 0, 0);
		count++;
	}
	if (!(events_[sockfd] & reactor::out) && (flags & reactor::out)) {
		EV_SET(&evs[count], sockfd, EVFILT_WRITE, EV_ADD, 0, 0, 0);
		count++;
	}

	if (count > 0) {
		if (::kevent(kev_, evs, count, 0, 0, 0) < 0)
			throw errno_exception("select", sio::system_errno());
	}

	events_[sockfd] = flags;
}

int sio::kqueue_reactor::poll(sio::reactor::event* events,
		const int& n, const int& timeout)
{
	struct ::kevent evs[128] = { 0 };
	struct ::timespec tv, *ptv = 0;

	if (timeout >= 0) {
		tv.tv_sec = timeout / 1000;
		tv.tv_nsec = (timeout % 1000) * 1000000L;
		ptv = &tv;
	}

	int read = ::kevent(kev_, 0, 0, evs, 128, ptv);
	if (read < 0)
		throw errno_exception("poll", sio::system_errno());

	int count = 0;
	for (int i = 0; i < read; i++) {
		if (evs[i].flags & EV_EOF)
			add(evs[i].ident, events, n, count, reactor::in);
		if (evs[i].flags & EV_ERROR)
			add(evs[i].ident, events, n, count, reactor::err);
		if (evs[i].filter == EVFILT_READ)
			add(evs[i].ident, events, n, count, reactor::in);
		if (evs[i].filter == EVFILT_WRITE)
			add(evs[i].ident, events, n, count, reactor::out);
	}

	return count;
}

void sio::kqueue_reactor::add(const sio::socket_t& sockfd,
		sio::reactor::event* events, const int& n,
		int& count, const int& flags)
{
	for (int i = 0; i < count; i++) {
		if (events[i].sockfd == sockfd) {
			events[i].events |= flags;
			return;
		}
	}

	if (count < n) {
		events[count].sockfd = sockfd;
		events[count].events = flags;
		count++;
	}
}
#endif

#if defined(SIO_HAVE_WIN32_SELECT)
sio::win32_select_reactor::win32_select_reactor(void)
{
}

std::shared_ptr<sio::reactor_impl> sio::win32_select_reactor::create(void)
{
	return std::make_shared<win32_select_reactor>();
}

void sio::win32_select_reactor::select(const sio::socket_t& sockfd,
		const int& flags)
{
	if (sockfd == invalid_socket)
		throw errno_exception("select", EBADF);
	if (flags == 0)
		pollfds_.erase(sockfd);
	else
		pollfds_[sockfd] = flags;
}

int sio::win32_select_reactor::poll(sio::reactor::event* events,
		const int& n, const int& timeout)
{
	fdset_t rd, wr, er;
	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_ZERO(&er);
	int count = 0;

	for (pollfds_type::iterator p = pollfds_.begin(); p != pollfds_.end(); ++p)
	{
		int flags = p->second;
		if (flags == 0)
			continue;

		socket_t sockfd = p->first;
		if (++count >= FD_SETSIZE)
			throw errno_exception("poll", EINVAL);

		if (flags & reactor::in)
			set(sockfd, &rd);
		if (flags & reactor::out)
			set(sockfd, &wr);
		set(sockfd, &er);
	}

	timeval_t tv, *ptv = nullptr;
	if (timeout >= 0) {
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		ptv = &tv;
	}

	int read = ::select(0, &rd, &wr, &er, ptv);
	if (read < 0)
		throw errno_exception("poll", sio::socket_errno());

	std::sort(rd.fd_array, rd.fd_array + rd.fd_count);
	std::sort(wr.fd_array, wr.fd_array + wr.fd_count);
	std::sort(er.fd_array, er.fd_array + er.fd_count);

	count = 0;

	for (pollfds_type::iterator p = pollfds_.begin(); read > 0 &&
		p != pollfds_.end(); ++p)
	{
		sio::socket_t sockfd = p->first;
		bool r = is_set(sockfd, &rd);
		read -= int(r);
		bool w = is_set(sockfd, &wr);
		read -= int(w);
		bool e = is_set(sockfd, &er);
		read -= int(e);
		if (r || w || e) {
			if (count >= n) {
				count++;
				continue;
			}
			reactor::event& ev = events[count];
			ev.sockfd = sockfd;
			ev.events = 0;
			if (r)
				ev.events |= reactor::in;
			if (w)
				ev.events |= reactor::out;
			if (e)
				ev.events |= reactor::err;
			count++;
		}
	}

	return count;
}
#endif

