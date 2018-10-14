#if defined(SIO_HAVE_POLL) || defined(SIO_HAVE_WSAPOLL)
SIO_INLINE std::string sio::poll_reactor::name(void) const
{
	return "poll";
}
#endif

#if defined(SIO_HAVE_DEVPOLL)
SIO_INLINE std::string sio::devpoll_reactor::name(void) const
{
	return "devpoll";
}
#endif

#if defined(SIO_HAVE_EPOLL)
SIO_INLINE std::string sio::epoll_reactor::name(void) const
{
	return "epoll";
}
#endif

#if defined(SIO_HAVE_POSIX_SELECT)
SIO_INLINE std::string sio::select_reactor::name(void) const
{
	return "select";
}
#endif

#if defined(SIO_HAVE_KQUEUE)
SIO_INLINE std::string sio::kqueue_reactor::name(void) const
{
	return "kqueue";
}
#endif

#if defined(SIO_HAVE_WIN32_SELECT)
SIO_INLINE std::string sio::win32_select_reactor::name() const
{
	return "select";
}

SIO_INLINE void sio::win32_select_reactor::set(
	const sio::socket_t& sockfd, sio::fdset_t* s)
{
	s->fd_array[s->fd_count++] = sockfd;
}

SIO_INLINE bool sio::win32_select_reactor::is_set(
	const sio::socket_t& sockfd, sio::fdset_t* s) const
{
	return std::binary_search(s->fd_array,
		s->fd_array + s->fd_count, sockfd);
}
#endif

