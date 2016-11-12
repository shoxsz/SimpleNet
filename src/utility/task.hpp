#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <functional>
#include <chrono>

namespace nsa{
	class Task{
	public:

		Task(const std::function<void(void)>& task) : task(task){}

		void operator()(){
			task();
		}

	private:
		std::function<void()> task;
		
		std::chrono::milliseconds timeout;
	};
};

#endif