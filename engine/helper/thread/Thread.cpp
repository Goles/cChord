#ifndef __Thread_CPP__
#define __Thread_CPP__

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <time.h>

#ifdef OSX
#include "TimeHelper.h"
#endif

#include "Thread.h"

#define u_int int

void micro_sleep(long micro) {
	usleep(micro);
}

void sec_sleep(long sec) {
	sleep(sec);
}

void Thread::start() {
	srand((long) pthread_self());
	int e = pthread_create(&this->theThread, &this->tattr, &_run, this);
	if (e != 0) {
		std::cout << "unable to create Thread : " << strerror(e) << std::endl;
		throw(new Exception("Unable to create Thread"));
	}
}

void Thread::join() {
	if (pthread_join(this->theThread, NULL) != 0) {
		throw(new Exception("Unable to join Thread"));
	}
}

int Thread::usleep(long micros) {

	int retval = -1;
	if (pthread_mutex_lock(&(this->sleep_mutex)) == 0) {
		struct timespec tm;
		clock_gettime(CLOCK_REALTIME, &tm);
		tm.tv_nsec = tm.tv_nsec + (micros * 1000.0);
		retval = pthread_cond_timedwait(&(this->cancel_cond),
				&(this->sleep_mutex), &tm);
		pthread_mutex_unlock(&(this->sleep_mutex));
	} else {
		std::cout << "usleep could not get the mutex" << std::endl;
	}
	return retval;
}

void Thread::sleep(long sec) {
	if (sec > 0) {
		sec_sleep(sec);
	} else {
		sleep();
	}
}

void Thread::wait() {
	if (pthread_mutex_lock(&(this->wait_mutex)) == 0) {
		pthread_cond_wait(&(this->notify_cond), &(this->wait_mutex));
		pthread_mutex_unlock(&(this->wait_mutex));
	}
}
void Thread::notify() {
	pthread_cond_signal(&(this->notify_cond));
}

#endif
