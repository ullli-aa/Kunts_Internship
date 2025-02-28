#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>

class Task {
public:
	virtual void Do() = 0;
	virtual ~Task() = default;
};

class LibThreadPool {
public:
	LibThreadPool(size_t numThreads = std::thread::hardware_concurrency()) : stop(false) {
		for (size_t i = 0; i < numThreads; i++)
		{
			threads.emplace_back([this] {
				while (!stop) {
					std::unique_ptr<Task> task;
					
					std::unique_lock<std::mutex> lock(queueMutex);
					condition.wait(lock, [this] { return stop || !tasks.empty(); });

					task = std::move(tasks.front());
					tasks.pop();

					lock.unlock();
					
					task->Do();
					condition.notify_all();
				}
			});
		}
	}

	~LibThreadPool() {
		StopPool();
	}

	void AddTask(std::unique_ptr<Task> task) {
		std::unique_lock<std::mutex> lock(queueMutex);
		tasks.push(std::move(task));
		lock.unlock();
		
		condition.notify_one();
	}

	void WaitForFinish() {
		std::unique_lock<std::mutex> lock(queueMutex);
		condition.wait(lock, [this] { return tasks.empty() || stop; });
	}

	void StopPool() {
		stop = true;

		condition.notify_all();
		for (std::thread& thread : threads) {
			thread.join();
		}
	}

	size_t NumThreads() const {
		return threads.size();
	}

private:
	std::vector<std::thread> threads;
	std::queue<std::unique_ptr<Task>> tasks;
	std::mutex queueMutex;
	std::condition_variable condition;
	std::atomic<bool> stop;
};

//у таска айди и считать колво обработанных и если обр = ласт инд то финиш
// заводить разные кондитионал вэрибл