#include "scheduler.hpp"

using namespace nsa;

void Scheduler::addTask(Task* task, bool pushFront){
	bool wasEmpty = false; 

	mutex.lock();

	if (threadState == RUNNING){
		wasEmpty = tasks.empty();
		if (pushFront)
			tasks.push_front(task);
		else
			tasks.push_back(task);
	}
	else{
		delete task;
	}

	mutex.unlock();

	if (wasEmpty)
		signal.notify_one();
}

void Scheduler::run(){
	std::unique_lock<std::mutex> locker(mutex, std::defer_lock);

	while (threadState == RUNNING){
		if (tasks.empty())
			signal.wait(locker);//wait a task to be added

		if (threadState == RUNNING){
			locker.lock();
			Task* task = tasks.front();
			tasks.pop_front();
			locker.unlock();
			(*task)();
			delete task;
		}
	}
}

void Scheduler::internalFlush(){
	while (!tasks.empty()){
		Task* task = tasks.front();
		tasks.pop_front();

		(*task)();
		delete task;
	}
}