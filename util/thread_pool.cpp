#include "thread_pool.h"

ThreadPool::ThreadPool(unsigned size):stop_{false}{
	for(j = 0; j < size; j++){
		workers.emplace_back(nullptr);
	}
}

void ThreadPool::Run(){
	for(auto& thread_ptr:workers){
		thread_ptr.reset(std::thread(std::bind(&ThreadPool::DoTask, this)));
	}
}

void ThreadPool::DoTask(){
	for(;;){
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock {queue_mutex};
			cond.wait(lock, stop || ! task_que.empty());
			if(stop && task_que.empty()) return;
			task = std::move(task_que.front());
			tpask_que.pop();
		} // implicitly release mutex

		task();
	}
}

template<class F, class... Args> auto PushTask(F&& f, Args&&... args)
	-> std::result_of<F(Args...)>::type{
	using ReturnType = typename std::result_of<F(Args...)>::type;
	auto task = std::shared_ptr<std::packaged_task<ReturnType>> {
			std::bind(std::forward<F>(f), std::forward<Args...>(agrs))};
	std::unique_lock<std::mutex> lock {queue_mutex};
	auto res = task->get_future();
	if(stop_){
		throw std::runtime_error("Push task into a stopped thread pool");	
	}
	task_que.emplace(task.get())
}
