#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <glog/logging.h>

#include "server.h"

Server::Server(const std::string& address, 
	const int& port,
	const int& thread_size,
	const int& maxmsg):mms{maxmsg},
	thread_pool_(thread_size){
	DLOG(INFO) << "Create socket";
	CreateSocket();
	
	DLOG(INFO) << "Bind socket";
	Bind(address, port);

	DLOG(INFO) << "Listen socket";
	Listen();
}

void Server::Run(){
	int epfd = epoll_create(SIZE);

	DLOG(INFO) << "Handle accept";
	std::thread acceptor(std::bind(&Server::AcceptHandler, this, epfd));

	DLOG(INFO) << "Handle vent";
	EventHandler(epfd);
}

void Server::CreateSocket(){
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0){
		DLOG(INFO) << "socket error: " << strerror(errno);
		abort();
	}
}

void Server::Bind(const std::string& address, const int& port){
	struct sockaddr_in sock_addr;
	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	if(address == ""){
		sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else{
		const char* ip = address.c_str();
		if(inet_pton(AF_INET, ip, &sock_addr.sin_addr)  <= 0){
			DLOG(INFO) << "inet_pton error: " << strerror(errno);
			abort();
		}
	}
	if(bind(listenfd, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0){
		DLOG(INFO) << "bind socket error: " << strerror(errno);
		abort();
	}
}

void Server::Listen(){
	if(listen(listenfd, LISTENQ) < 0){
		DLOG(INFO) << "listen error: " << strerror(errno);
		abort();
	}
}

 void Server::AcceptHandler(const int& epfd){
	struct epoll_event ev;
	ev.events = EPOLLIN;
	while (true){
		int connfd;
		if((connfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) < 0){
			DLOG(INFO) << "accept error: " << strerror(errno);
			abort();
		}
		ev.data.fd = connfd;
		epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
	}
}

void Server::EventHandler(const int& epfd){	
	while(true){
		int nfd = epoll_wait(epfd, events, MAXEVENTS, TIMEOUT);
		for(int j = 0; j < nfd; j++){
			int connfd = events[j].data.fd;
			epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, events+j);
			thread_pool_.enqueue(std::bind(&Server::CalcService, this, connfd));
		}
	}
}

void Server::CalcService(const int& fd){
	char* buf = new char[mms];
	union u{
		int num;
		char str[sizeof(int)];
	} len;
	len.num = read(fd, buf, mms);
	write(fd, len.str, sizeof(int));
	delete buf;
	buf = nullptr;
	close(fd);
}

