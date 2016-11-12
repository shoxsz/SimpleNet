#ifndef _SCHEDULER_HPP_
#define _SCHEDULER_HPP_

#include "task.hpp"
#include "thread.hpp"

#include <mutex>
#include <list>
#include <condition_variable>

namespace nsa{
	class Scheduler : public Thread{
	public:

		Scheduler(const Scheduler& scheduler) = delete;
		Scheduler& operator=(const Scheduler& scheduler) = delete;
		
		Scheduler(){}
		
		~Scheduler(){
			flush();
			join();
		}

		void addTask(Task* task, bool pushFront = false);

		void flush(){
			addTask(new Task(std::bind(&Scheduler::internalFlush, this)));
		}

		void sleep(std::chrono::milliseconds milli){
			addTask(new Task(std::bind(&Scheduler::internalSleep, this, milli)));
		}

		void run();

	private:
		void internalSleep(std::chrono::milliseconds milli){
			std::this_thread::sleep_for(milli);
		}
		void internalFlush();

		std::list<Task*> tasks;
		std::mutex mutex;
		std::condition_variable signal;
	};
};

#endif