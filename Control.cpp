#include <glog/logging.h>

#include "control.h"
#include "server.h"

static Control& Control::GetInstance(){
	static Control instance;
	return instance;
}

void Control::Start(){
	Server server {args};
	server.Run();
}

void Control::Stop(){
	//pass
}

Control::Control()exit:{false}{}
