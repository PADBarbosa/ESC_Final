#include <mutex>
#include <condition_variable>


class Barrier {

public:
    Barrier(int thread_count);
    void wait(void);

private:
    std::mutex counterMutex;
    std::condition_variable cond;

    int thread_count;
    int current;
};



Barrier::Barrier(int arg_thread_count) {

    thread_count = arg_thread_count;
    current = 0;
}


void Barrier::wait(void){
	std::unique_lock<std::mutex> lock(counterMutex);
	current++;

	if(current < thread_count){
		cond.wait(lock);
	}
	else{
		cond.notify_all();
		current = 0;
	}

}