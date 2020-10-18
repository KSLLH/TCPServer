#include <unistd.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "init_google.h"

void FailureWriter(const char * data, int size){
	write(STDERR_FILENO, data, size);
	google::base::GetLogger(google::FATAL)->Write(true, time(NULL), data, size);
}

void InitGoogle(int * argc, char * * * argv){
	google::ParseCommandLineFlags(argc, argv, true);
	google::InitGoogleLogging((*argv)[0]);
	google::InstallFailureSignalHandler();
	google::InstallFailureWriter(&FailureWriter);
}
