#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>

class Task {
public:
	Task() : id(0) {}
	virtual void Do() = 0;
	virtual ~Task() = default;

protected:
	int id;
};

class LibThreadPool {
public:
	LibThreadPool(size_t numThreads = std::thread::hardware_concurrency()) : 
		stop(false), LastId(0), complete(0) {
		for (size_t i = 0; i < numThreads; i++)
		{
			threads.emplace_back([this] {
				while (!stop) {
					std::unique_ptr<Task> task;
					
					std::unique_lock<std::mutex> lock(queueMutex);
					cv_add.wait(lock, [this] { return stop || !tasks.empty(); });

					if (stop) {
						return;
					}

					task = std::move(tasks.front());
					tasks.pop();

					lock.unlock();

					task->Do();
					complete++;

					cv_wait.notify_all();
				}
			});
		}
	}

	~LibThreadPool() {
		StopPool();
	}

	void AddTask(std::unique_ptr<Task> task) {
		std::unique_lock<std::mutex> lock(queueMutex);

		LastId++;
		tasks.push(std::move(task));
		lock.unlock();
		
		cv_add.notify_one();
	}

	void WaitForFinish() {
		std::unique_lock<std::mutex> lock(waitMutex);
		cv_wait.wait(lock, [this] { return tasks.empty() && complete == LastId; });
	}

	void StopPool() {
		stop = true;

		cv_add.notify_all();
		for (std::thread& thread : threads) {
			thread.join();
		}
	}

private:
	std::vector<std::thread> threads;
	std::queue<std::unique_ptr<Task>> tasks;
	std::mutex queueMutex;
	std::condition_variable cv_add;

	std::mutex waitMutex;
	std::condition_variable cv_wait;

	int LastId;
	std::atomic<bool> stop;
	std::atomic<int> complete;

};
