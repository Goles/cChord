/*
 *  MyThread.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/18/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
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

class MyThread : public Thread {
private:
	
	int id;
	ChordNode *myNode;
	
public:
	
	MyThread(int i) : Thread() {
		this->id = i;
	}

	MyThread(int i, ChordNode *node)
	{
		this->id = i;
		myNode = node;
	}
	
	void run()
	{
		/*Still have to implement a try-catch here*/
		if(myNode != NULL)
		{
			while(myNode->getAlive())
			{
				cout << "- stabilize -\n";
				myNode->stabilize();
				myNode->fixFingersTable();
				myNode->printStatus();
				this->sleep(myNode->getTimeToCheck());
			}			
		}
	}
};

#endif
