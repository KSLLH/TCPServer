#include <fcntl.h>
#include <unistd.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "init_google.h"
#include "control.h"

inline void InitSignal(){
	// do nothing
	return;
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


DEFINE_bool(daemon, false, "True if run as a daemon process.");


int main(int argc, char* argv[]){
	InitGoogle(&argc, & argv);	
	DLOG(INFO) << "Initialize glags and glog.";

	// Handle signal
	DLOG(INFO) << "Handle signals.";
	InitSignal();

	if(FLAGS_daemon){
		DLOG(INFO) << "Daemonize process.";
		Daemonize();
	}

	DLOG(INFO) << "Start Control";
	Control::GetInstance().Start();
}
