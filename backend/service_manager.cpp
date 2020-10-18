#include <gflags/gflags.h>
#include <glog/logging.h>

#include "service_manager.h"
#include "server.h"

DEFINE_string(address, "", "IP address of server");
DEFINE_int32(port, 2222, "Port number the server listens on");
DEFINE_int32(thread_size, 1, "Size of thread pool");

ServiceManager& ServiceManager::GetInstance(){
	static ServiceManager instance;
	return instance;
}

void ServiceManager::Start(){
	DLOG(INFO) << "Instantiate server";
	Server server {FLAGS_address, FLAGS_port, FLAGS_thread_size};
	DLOG(INFO) << "Run server";
	server.Run();	
}

ServiceManager::ServiceManager(){}
