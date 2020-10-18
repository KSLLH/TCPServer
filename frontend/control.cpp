#include <glog/logging.h>

#include "control.h"
#include "service_manager.h"

Control& Control::GetInstance(){
	static Control instance;
	return instance;
}

void Control::Start(){
	DLOG(INFO) << "Start ServiceManager";
	ServiceManager::GetInstance().Start();
}

void Control::Stop(){
	//pass
}

Control::Control(){}
