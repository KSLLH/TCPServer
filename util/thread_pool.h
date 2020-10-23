#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <memory>
#include <functional>
#include <furture>
#include <stdexcept>


class ThreadPool{
// basic operations
public:
	ThreadPool(unsigned size);
	ThreadPool(const ThreadPool& thread_pool) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	~ThreadPool();

// APIs
public:
	void Run();

	template<class F, class... Args> 
	auto PushTask(F&& f, Args&&... args)
	-> std::future<typename std::result_of<F(Args...)>::type>;

	bool TasksDone();
	
	unsigned Size();

// Internal method
private:
	void DoTask();

// Internal data
private:
	std::vector<std::shared_ptr<std::thread>> workers;
	std::queue<std::function<void()>> task_que;
	std::mutex queue_mutex;
	std::condition_variable cond;
	bool stop_;
	
};

#endif
