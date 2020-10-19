#ifndef SERVER_H
#define SERVER_H

#include <sys/epoll.h>

#include <string>

#include "util/thread_pool.h"

class Server{
public:
	Server(const std::string& address, 
		const int& port,
		const int& thread_size,
		const int& maxmsg);
	
	void Run();

private:
	void CreateSocket();

	void Bind(const std::string& address, const int& port);

	void Listen();

	void AcceptHandler(const int& epfd);

	void EventHandler(const int& epfd);

	void CalcService(const int& fd);

private:
	static const int SIZE {1000};
	static const int LISTENQ {200};
	static const int MAXEVENTS {1000};
	static const int TIMEOUT {10};

	int listenfd;
	int mms;
	struct epoll_event events[MAXEVENTS];
	ThreadPool thread_pool_;

};

#endif
