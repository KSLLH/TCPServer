#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <string>

#include "client.h"

Client::Client(const std::string & addr, 
	const int& p, 
	const int& thread_size,
	const int& maxmsg):
	address{addr},port{p},thread_pool_(thread_size),mms{maxmsg}{}


void Client::Run(){
	for(;;){
		thread_pool_.enqueue(std::bind(&Client::Task, 
			this, address, port));
	}
}

void Client::Task(const std::string& address, const int& port){
	int sockfd = CreateSocket();
	Connect(sockfd);
	CallCalcService(sockfd);
}


int Client::CreateSocket(){
	if((int sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		abort();
	}
	else {
		return sockfd;
	}
}

void Client::Connect(const int& sockfd){
	struct sockaddr_in sock_addr;
	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	const char* ip = address.c_str();
	if(inet_pton(AF_INET, ip, &sock_addr.sin_addr) <= 0){
		abort();
	}

	if(connect(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0){
		abort();
	}
	return;
}

void Client::CallCalcService(const int& sockfd, const std::string& str){
	union u{
		int num;
		char str[sizeof(int)];
	} ans;
	char* buf = new char[maxmsg];
	bzero(buf, sizeof(buf));
	int nbyte;
	for(nbyte = 0; nbyte < 20 && nbyte < str.size(); nbyte++){
		buf[nbyte] = str[nbyte];
	}
	write(sockfd, buf, nbyte);
	read(sockfd, ans.str, sizeof(int));
	if(str.size() != ans.num) abort();
}
