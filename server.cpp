#include "server.h"

Server::Server(const string& address, const int port){
		acceptor_.Bind(address, port);
		acceptor_.Listen();
	};

Server::Run(){
		int epfd = epoll_create(SIZE);
		std::thread t(&Acceptor::AcceptHandler, &acceptor_, epfd);
		reactor_.EventHandle(epfd);
	};

Server::Stop(){}