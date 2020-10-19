#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "client.h"

Client::Client(const std::string & addr, 
	const int& port, 
	const int& thread_size,
	const int& maxmsg,
	const int& interval):
		address{addr},
		port{port},
		thread_pool_(thread_size),
		mms{maxmsg},
		task_interval{interval}{
	DLOG(INFO) << "Instantiatin client...";
}


void Client::Run(){
	for(;;){
		DLOG(INFO) << "New Task Start:";
		thread_pool_.enqueue(std::bind(&Client::Task, 
			this, address, port));
		std::this_thread::sleep_for(std::chrono::milliseconds(task_interval));
		DLOG(INFO) << "Task done.";
	}
}

void Client::Task(const std::string& address, const int& port){
	DLOG(INFO) << "Start New TCP Connection:";
	int sockfd = TCPConnection(address, port);
	DLOG(INFO) << "New TCP connected.";
	std::string str = GenerateMsg(); 
	DLOG(INFO) << "Message:  " << str << " generated.";
	CallCalcService(sockfd, str);
	DLOG(INFO) << "Service returned for message=" << str << ".";
}

int Client::TCPConnection(const std::string& address, const int& port){
	int sockfd;
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
		DLOG(INFO) << "socket error: " << strerror(errno);
		abort();
	}
	DLOG(INFO) << "Socket created.";
	struct sockaddr_in sock_addr;
	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	const char* ip = address.c_str();
	DLOG(INFO) << "IP=" << address << " port=" << port;
	if( inet_pton(AF_INET, ip, &sock_addr.sin_addr) <= 0){
		DLOG(INFO) << "inet_pton error: " << strerror(errno);
		abort();
	}
	DLOG(INFO) << "Server socket address obtained.";

	if( connect(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0){
		DLOG(INFO) << "connect error: " << strerror(errno);
		abort();
	}
	DLOG(INFO) << "Connected to server.";
	return sockfd;
}

void Client::CallCalcService(const int& sockfd, const std::string& str){
	char* buf = new char[str.size()];
	for(int j = 0; j < str.size(); j++){
		buf[j] = str[j];
	}
	DLOG(INFO) << "Message " << str << " written into buffer.";

	
	DLOG(INFO) << write(sockfd, buf, str.size()) << " bytes written into socket.";

	union u{
		int num;
		char str[sizeof(int)];
	} response;
	read(sockfd, response.str, sizeof(int));
	DLOG(INFO) << "Answer=" << response.num << " responsed.";

	delete buf;
	buf = nullptr;

	if(str.size() != response.num){
		DLOG(WARNING) << "Wrong answer. Expect output=" 
				   << str.size() << ", actual output=" 
				   << response.num;
	}
	else{
		DLOG(INFO) << "Answer correct, well done.";
	}
}

std::string Client::GenerateMsg(){
	int msg_len = rand() % mms + 1;
	char* msg = new char[msg_len + 1];
	
	for(int j = 0; j < msg_len; j++){
		msg[j] = rand() % 128;
		if(msg[j] < 32) msg[j] = 32;
		if(msg[j] > 126) msg[j] = 126;
	}
	msg[msg_len] = '\0';
	
	std::string str {msg};
	
	delete msg;
	msg = nullptr;
	
	return str;
}
