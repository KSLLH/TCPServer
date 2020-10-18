#include "acceptor.h"

Acceptor::Acceptor(){
	fd = socket(AF_INET, SOCK_STREAM, 0);
}

void Acceptor::Bind(const string & address, const int port){
	struct sockaddr_in sock_addr;
	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	if(address == ""){
		sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else{
		const char* ip = address.c_str();
		inet_pton(AF_INET, address, &sock_addr.sin_addr);
	}
	bind(fd, (struct sockaddr*) &sock_addr, sizeof(sock_addr));
	
}

void Acceptor::Listen(){
	listen(fd, LISTENQ);
}

void Acceptor::AcceptHandler(const int epfd){
	struct epoll_event ev;
	ev.events = EPOLLIN|EPOLLONESHOT;
	while (true){
		int connfd = accept(fd, (struct sockaddr*) NULL, NULL);
		ev.data.fd = connfd;
		epoll_ctl(epfd, EPOLL__CTL_ADD, connfd, &ev);
	}
}
