#include <fcntl.h>
#include <unistd.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "util/init_google.h"
#include "util/thread_pool.h"

DEFINE_string(address, "10.128.134.184", "IP address of server");
DEFINE_int32(port, 2222, "port of serever");
DEFINE_bool(daemon, false, "True if run as a daemon process.");
DEFINE_int32(workers, 8, "Number of processes");
DEFINE_int32(thread_size, 100, "Number of threads per process");

int main(int argc, char* argv[]){
	InitGoogle(&argc, & argv);	
	DLOG(INFO) << "Initialize glags and glog.";

	if(FLAGS_daemon){
		DLOG(INFO) << "Daemonize process.";
		Daemonize();
	}
	
	for(int j = FLAGS_workers; j > 0; j = j >> 1){
		fork();
	}

	TreadPool thread_pool {FLAGS_thread_size};
	for(;;) {
		thread_pool.enqueue(CallCalcService);
	}
}

inline void Daemonize(){
	if(0 != fork()){
		exit(0);
	}
	setsid();
	int fd = open("/dev/null", O_RDWR, 0);
	if(-1 != fd){
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		if(fd > STDERR_FILENO){
			close(fd);
		}
	}
	return;
}

int Task(const string& address, const int& port, const string& str){
	int sockfd = TCPConnection(address, port);
	return CallCalcService(sockfd,str);
}

int TCPConnection(const string& address, const int& port){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sock_addr;
	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	inet_pton(AF_INET, address.c_str(), &sock_addr.sin_addr);
	connect(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
	return sockfd;
}

int CallCalcService(const int& sockfd, const string& str){
	int ans;
	char buf[20];
	int nbyte;
	for(nbyte = 0; nbyte < 20 || nbyte < str.size(); nbyte++){
		buf[nbyte] = str[nbyte];
	}
	write(sockfd, buf, nbyte);
	read(sockfd, (char*)&ans, sizeof(int));
	return res;
}
