#include <gflags/gflags.h>
#include <glog/logging.h>

#include "service_manager.h"
#include "server.h"

DEFINE_string(address, "", "IP address of server");
DEFINE_int(port, 2222, "Port number the server listens on");
DEFINE_int(thread_size, 100, "Size of thread pool");

static ServiceManager& ServiceManager::GetInstance(){
	static ServiceManager instance;
	return instance;
}

void ServiceManager::Start(){
	DLOG(info) << "Instantiate server";
	Server server {FLAGS_address, FLAGS_port, FLAGS_thread_size};
	DLOG(info) << "Run server";
	server.Run();	
}

ServiceManager::ServiceManager(){}
