#ifndef REACTOR_H
#define REACTOR_H

#include <stdlib.h> 
#include <sys/epoll.h>

#include "thread_pool.h"

class Reactor{
public:
	Reactor(const int threads);

	void EventHandler(const int epfd);

	static void CalcService(const int fd);
	
private:
	ThreadPool thread_pool_;
	const int MAXEVENTS {1000};
	const int TIMEOUT {50};
	struct epoll_event events[MAXEVENTS];
};
#endif
