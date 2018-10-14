#ifndef __SIO_REACTOR_IMPL__
#define __SIO_REACTOR_IMPL__

#include <vector>
#include <memory>

#include "sio.reactor.h"

namespace sio
{
#if defined(SIO_HAVE_POLL) || defined(SIO_HAVE_WSAPOLL) || defined(SIO_HAVE_DEVPOLL)
	class base_poll_reactor : public reactor_impl
	{
	protected:
		int to_poll_events(const int& event);
		int to_user_events(const int& event);
	};
#endif

#if defined(SIO_HAVE_POLL) || defined(SIO_HAVE_WSAPOLL)
	class poll_reactor : public base_poll_reactor
	{
		std::vector<std::size_t> map_;
		std::vector<::pollfd> pollfds_;
	public:
		poll_reactor(void);
	public:
		static std::shared_ptr<reactor_impl> create(void);
	public:
		SIO_INLINE std::string name(void) const;
	public:
		void select(const socket_t& sockfd, const int& flags);
		int poll(reactor::event* events, const int& n, const int& timeout);
	private:
		struct ::pollfd& entry(const socket_t& sockfd);
		void remove(const socket_t& sockfd);
	};
#endif


#if defined(SIO_HAVE_DEVPOLL) || defined(SIO_HAVE_EPOLL) || defined(SIO_HAVE_KQUEUE)
	class base_fast_reactor
	{
	protected:
		std::vector<socket_t> events_;
		void check(const socket_t& sockfd);
	};
#endif

#if defined(SIO_HAVE_DEVPOLL)
	class devpoll_reactor : public base_poll_reactor,
		public base_fast_reactor
	{
		int pollfd_;
	public:
	public:
		devpoll_reactor(void);
		virtual ~devpoll_reactor(void);
	public:
		static std::shared_ptr<reactor_impl> create(void);
	public:
		SIO_INLINE std::string name(void) const;
	public:
		void select(const socket_t& sockfd, const int& flags);
		int poll(reactor::event* events, const int& n, const int& timeout);
	};
#endif

#if defined(SIO_HAVE_EPOLL)
	class epoll_reactor : public base_poll_reactor,
		public base_fast_reactor
	{
		int pollfd_;
	public:
		epoll_reactor(void);
		virtual ~epoll_reactor(void);
	public:
		static std::shared_ptr<reactor_impl> create(void);
	public:
		SIO_INLINE std::string name(void) const;
	public:
		void select(const socket_t& sockfd, const int& flags);
		int poll(reactor::event* events, const int& n, const int& timeout);
	private:
		int to_poll_events(const int& event);
		int to_user_events(const int& event);
		void write_flag(const socket_t& sockfd,
				const int& op, const int& flags);
	};
#endif

#if defined(SIO_HAVE_POSIX_SELECT)
	class select_reactor : public reactor_impl
	{
		struct event
		{
			socket_t sockfd;
			int events;
		};
		std::vector<int> map_;
		std::vector<event> pollfds_;
	public:
		select_reactor(void);
	public:
		static std::shared_ptr<reactor_impl> create(void);
	public:
		SIO_INLINE std::string name(void) const;
	public:
		void select(const socket_t& sockfd, const int& flags);
		int poll(reactor::event* events, const int& n, const int& timeout);
	private:
		event& entry(const socket_t& sockfd);
		void remove(const socket_t& sockfd);
	};
#endif

#if defined(SIO_HAVE_KQUEUE)
	class kqueue_reactor : public reactor_impl,
		public base_fast_reactor
	{
		int kev_;
	public:
		kqueue_reactor(void);
		virtual ~kqueue_reactor(void);
	public:
		static std::shared_ptr<reactor_impl> create(void);
	public:
		SIO_INLINE std::string name(void) const;
	public:
		void select(const socket_t& sockfd, const int& flags);
		int poll(reactor::event* events, const int& n, const int& timeout);
	private:
		void add(const socket_t& sockfd, reactor::event* events,
				const int& n, int& count, const int& flags);
	};
#endif

#if defined(SIO_HAVE_WIN32_SELECT)
	class win32_select_reactor : public reactor_impl
	{
		typedef std::map<socket_t, int> pollfds_type;
		pollfds_type pollfds_;
	public:
		win32_select_reactor(void);
	public:
		static std::shared_ptr<reactor_impl> create(void);
	public:
		SIO_INLINE std::string name() const;
	public:
		void select(const socket_t& sockfd, const int& flags);
		int poll(reactor::event* events, const int& n, const int& timeout);
	private:
		SIO_INLINE void set(const socket_t& sockfd, fdset_t* s);
		SIO_INLINE bool is_set(const socket_t& sockfd, fdset_t* s) const;
	};
#endif
}

#if defined(__SIO_INLINE__)
#include "sio.reactor_impl.inl"
#endif

#endif

