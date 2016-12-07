#include <mutex>
#include <condition_variable>

class Semaphore
{
	public:
		Semaphore(int count_ = 0) : count(count_){}

		/*leaving critical section and wake up someone who is waiting*/
		void signal()
		{
			std::unique_lock<std::mutex> lck(mtx);	//acquire lock
			++count;
			cv.notify_one();	//It wakes up another one sleeping
		}

		/*waiting for enter critical section*/
		void wait()
		{
			std::unique_lock<std::mutex> lck(mtx);	//acquire lock
			while(count == 0)	//if count==0,which means someone is in its
			{					//critical section,so that others have to 
				cv.wait(lck);	//wait.When they in this line,they release
			}					//the lock.

			--count;			//If someone is notified so it leave the
		}						//while-loop,they reacquire the lock and
								//count--.

	private:
		std::mutex mtx;
		std::condition_variable cv;
		int count;
};
