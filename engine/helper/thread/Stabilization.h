/*
 *  MyThread.h
 *  iPhone_p2p_engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

/*
 *	Inherits from "Thread" (POSIX compilant thread implementation)
 *	the run method is what actually runs the checkStable().
 *	_NG
 */

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "Thread.h"
#include "ChordNode.h"

class Stabilization: public Thread {
private:

	int id;
	ChordNode *myNode;

public:

	Stabilization(int i) :
		Thread() {
		this->id = i;
	}

	Stabilization(int i, ChordNode *node) {
		this->id = i;
		myNode = node;
	}

	void run() {
		/*Still have to implement a try-catch here*/
		if (myNode != NULL) {
			while (myNode->getAlive()) {
				myNode->stabilize();
				myNode->fixFingersTable();
				myNode->checkPredecessor();
				// micro to milli second converter
				for (int i = 0; i < 1000; i++) {
					this->usleep(myNode->getTimeToCheck());
				}
			}
		}
	}
};

#endif
