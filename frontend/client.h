#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "util/thread_pool.h"

class Client{
public:
	Client(const std::string& addr, 
		const int& port,
		const int& thread_size,
		const int& maxmsg);
	
	void Run();

private:
	void Task(const std::string& addr, const int& port);

	int TCPConnection(const std::string& addr, const int& port);
	
	std::string GenerateMsg(const int& maxmsg);

	void CallCalcService(const int& fd, const std::string& str);

private:
	int mms;
	int port;
	std::string address;
	ThreadPool thread_pool_;
};
#endif
