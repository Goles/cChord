#ifndef __Thread__
#define __Thread__

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <iostream>

#include "Exception.h"

class Thread {

public:
private:

	pthread_t theThread;
	pthread_attr_t tattr;
	pthread_mutex_t mutex;

	pthread_cond_t cancel_cond;
	pthread_mutex_t sleep_mutex;

	pthread_cond_t notify_cond;
	pthread_mutex_t wait_mutex;

	bool done;

protected:

	inline static void* _run(void* arg) {
		Thread* t = (Thread*) arg;
		t->thePid = getpid();
		pthread_testcancel();
		t->run();
		t->done = true;
		pthread_exit(0);
		return t;
	}

public:
	int thePid;

	Thread() {
		this->theThread = 0;

		pthread_mutex_init(&(this->mutex), 0);

		pthread_mutex_init(&(this->wait_mutex), 0);
		pthread_cond_init(&(this->notify_cond), 0);

		pthread_mutex_init(&(this->sleep_mutex), 0);
		pthread_cond_init(&(this->cancel_cond), 0);

		if (pthread_attr_init(&this->tattr) != 0) {
			throw(new Exception("could not initialize the pthread attr"));
		}
		this->done = false;
	}

	~Thread() {
		//std::cout << "thread destructor" << std::endl;
		pthread_attr_destroy(&(this->tattr));
		pthread_mutex_destroy(&(this->mutex));

		pthread_mutex_destroy(&(this->wait_mutex));
		pthread_cond_destroy(&(this->notify_cond));

		pthread_mutex_destroy(&(this->sleep_mutex));
		pthread_cond_destroy(&(this->cancel_cond));
	}

	pthread_t getThreadID() {
		return (pthread_self());
	}

	bool getMutex() {

		//cout << "thread " << this->getThreadID() << " getting mutex" <<  endl;
		if (pthread_mutex_lock(&(this->mutex)) != 0) {
			throw(new Exception("could not get the mutex"));
			//return false;
		}
		return true;
	}

	void releaseMutex() {
		//cout << "thread " << this->getThreadID() << " releasing mutex" << endl;
		if (pthread_mutex_unlock(&(this->mutex)) != 0) {
			throw(new Exception("could not release the mutex"));
		}
	}

	bool isDone() {
		return this->done;
	}

	void setDetached() {
		if (pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED) != 0) {
			throw(new Exception("could not set detached state"));
		}
	}

	virtual void run() {
	}

	void start();
	void join();
	int usleep(long micros);
	void sleep(long sec = 0);
	void wait();
	void notify();
	inline void kill() {
		this->done = true;
		if (this->theThread != 0) {
			if (pthread_cancel(this->theThread) != 0) {
				throw(new Exception("Unable to cancel the Thread"));
			}
		}
	}

	bool signal(int signal) {
		if (pthread_kill(this->theThread, signal) != 0) {
			return false;
		}
		return true;
	}

	inline bool cancel() {
		if (pthread_cond_signal(&(this->cancel_cond)) != 0) {
			return false;
		}
		return true;
	}

};

#endif
