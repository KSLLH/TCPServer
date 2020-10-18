#include <unistd.h>

#include "reactor.h"

Reactor::Reactor(const int threads):thread_pool_(threads){}

void Reactor::EventHandler(const int epfd){
	while(true){
		epoll_wait(epfd, events, MAXEVENTS, TIMEOUT);
		for(auto& ev : events){
			thread_pool_.enqueue(Reactor::CalcService, ev.data.fd);
		}
	}
}

static void Reactor::CalcService(const int fd){
	const BUFSIZE {20};
	char buf[BUFSIZE];
	int len {0};
	while((len = read(fd, buf, BUFSIZE)) > 0);
	write(fd, (char*)&len, sizeof(int));
	close(fd);
}
