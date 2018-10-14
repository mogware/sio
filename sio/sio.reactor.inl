SIO_INLINE std::string sio::reactor::name(void) const
{
	return impl_->name();
}

SIO_INLINE void sio::reactor::select(const sio::socket_t& sockfd, const int& flags)
{
	impl_->select(sockfd, flags);
}

SIO_INLINE void sio::reactor::remove(const sio::socket_t& sockfd)
{
	impl_->select(sockfd, 0);
}

SIO_INLINE int sio::reactor::poll(sio::reactor::event* events,
	const int& n, const int& timeout)
{
	return impl_->poll(events, n, timeout);
}