#ifndef SERVER_H
#define SERVER_H

#include <sys/epoll.h>

#include <string>

#include "thread_pool.h"

class Server{
public:
	Server(const string& address, 
		const int port,
		const int thread_pool_size);
	
	void Run();

private:
	void CreateSocket();

	void Bind(const string& address, const int port);

	void Listen();

	void AcceptHandler(const int epfd);

	void EventHandler(const int epfd);

	void CalcService(const int fd);

private:
	const int SIZE {1000};
	const int LISTENQ {50};
	int listenfd;
	const int MAXEVENTS {1000};
	const int TIMEOUT {10};
	struct epoll_event events[MAXEVENTS];
	ThreadPool thread_pool_;

};

#endif
