#ifndef SERVER_H
#define SERVER_H

#include "reactor.h"
#include "acceptor.h"

class Server{
public:
	Server(const string& address, const int port);

	void Run();
	
	void Stop();

private:
	Acceptor acceptor_;
	Reactor reactor_;
	bool stop_ = false;
	const int SIZE {1000};

};

#endif
