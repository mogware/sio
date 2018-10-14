SIO_INLINE int sio::errno_exception::get_errno(void) const
{
	return errno_;
}

SIO_INLINE const char* sio::errno_exception::what(void) const noexcept
{
	return what_.c_str();
}
