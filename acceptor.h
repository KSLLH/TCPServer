#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

class Acceptor{
public:
	Acceptor();
	
	void Bind(const string& address, const int port);

	void Listen();

	void AcceptHandler(const int epfd);

private:
	const int LISTENQ = 1000;
	int fd;
};

#endif
