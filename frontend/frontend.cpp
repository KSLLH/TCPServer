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
#include "client.h"

DEFINE_string(address, "10.128.134.184", "IP address of server");
DEFINE_int32(port, 2222, "port of serever");
DEFINE_int32(workers, 1, "Number of processes");
DEFINE_int32(thread_size, 2, "Number of threads per process");
DEFINE_int32(maxmsg, 20, "Maximum length of a message");
DEFINE_int32(interval, 1000, "Time interval of subsequent tasks in the same thread");

int main(int argc, char* argv[]){
	InitGoogle(&argc, &argv);

	DLOG(INFO) << "################";
	DLOG(INFO) << "# PROGRAM START";
	DLOG(INFO) << "################";
	
	DLOG(INFO) << "Google Initialized.";

	pid_t ppid = getpid();
	for (int j = 1; j < FLAGS_workers; j++) {
		if (getpid() == ppid) {
	        fork();
	    }
	}

	Client client(FLAGS_address, FLAGS_port, FLAGS_thread_size, FLAGS_maxmsg, FLAGS_interval);
	DLOG(INFO) << "Instantiate client.";
	
	client.Run();
	DLOG(INFO) << "Process Start:";
	return 0;
}
