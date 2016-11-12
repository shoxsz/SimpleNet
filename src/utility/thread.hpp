#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include <thread>

namespace snet{
	class Thread{
	public:
		enum ThreadState{
			IDDLE = 0,
			RUNNING,
			STOPED
		};

		void start(){
			if (threadState == IDDLE || threadState == STOPED){
				threadState = RUNNING;
				onStart();
				thread = std::thread(&Thread::run, this);
			}
		}

		/*set the thread state to stop and wait until it's finished*/
		void stop(){
			if (threadState == RUNNING){
				threadState = STOPED;
				onStop();
				if (thread.joinable())
					thread.join();
			}
		}

		void join(){
			if (thread.joinable())
				thread.join();
		}

		ThreadState getState()const{ return threadState; }

	protected:
		virtual void onStart(){}
		virtual void run() = 0;
		virtual void onStop(){}

		ThreadState threadState;
		std::thread thread;
	};
};

#endif