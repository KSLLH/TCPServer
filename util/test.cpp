#include <iostream>
#include "thread_pool.h"

void func(int x){
	std::cout << "x = " << x << std::endl;
}

int main(int argc, char* argv[]){
	ThreadPool thread_pool {8};
	thread_pool.Run();
	std::cout << thread_pool.Size();
	for(int j = 0; j < 16; j++){
		thread_pool.PushTask(std::bind(func, j));
	}
	if(thread_pool.TasksDone()){
		std::cout << "Tasks Done!" << std::endl;
	}
	return 0;
}
