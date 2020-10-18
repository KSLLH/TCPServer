#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#include <iostream>
#include <string>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "util/init_google.h"
#include "util/thread_pool.h"

DEFINE_string(address, "10.128.134.184", "IP address of server");
DEFINE_int32(port, 2222, "port of serever");
DEFINE_bool(daemon, false, "True if run as a daemon process.");
DEFINE_int32(workers, 8, "Number of processes");
DEFINE_int32(thread_size, 100, "Number of threads per process");

int Task(const std::string& address, const int& port, const std::string& str);
int TCPConnection(const std::string& address, const int& port);
int CallCalcService(const int& sockfd, const std::string& str);


int main(int argc, char* argv[]){
	InitGoogle(&argc, &argv);
	DLOG(INFO) << "TEST TEXT1";
	DLOG(INFO) << "TEST TEXT2";
	return 0;
}

int Task(const std::string& address, const int& port, const std::string& str){
	DLOG(INFO) << "Start TCPConnection";
	int sockfd = TCPConnection(address, port);
	DLOG(INFO) << "Start CalCalcService";
	return CallCalcService(sockfd, str);
}

int TCPConnection(const std::string & address, const int& port){
	int sockfd;
	DLOG(INFO) << "Create socket";
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		LOG(ERROR) << "socket error: " << strerror(errno);
		abort();
	}

	DLOG(INFO) << "Define server socket address";
	struct sockaddr_in sock_addr;
	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	const char* ip = address.c_str();
	if(inet_pton(AF_INET, ip, &sock_addr.sin_addr) <= 0){
		LOG(ERROR) << "inet_pton error: " << strerror(errno);
		abort();
	}

	DLOG(INFO) << "Connect to server";
	if(connect(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0){
		LOG(ERROR) << "connect error: " << strerror(errno);
		abort();
	}

	return sockfd;
}

int CallCalcService(const int& sockfd, const std::string& str){
	union u{
		int num;
		char str[sizeof(int)];
	} ans;
	char buf[20];
	bzero(buf, sizeof(buf));
	int nbyte;
	for(nbyte = 0; nbyte == 19 || nbyte == str.size() - 1; nbyte++){
		buf[nbyte] = str[nbyte];
	}
	DLOG(INFO) << "Write " << buf <<  " to socket";
	write(sockfd, buf, nbyte);
	
	DLOG(INFO) << "Read from socket";
	read(sockfd, ans.str, sizeof(int));
	return ans.num;
}
