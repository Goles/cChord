/*
 *  CheckPred.h
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010

 *
 */

/*
 *	Inherits from "Thread" (POSIX compilant thread implementation)
 *	the run method is what actually runs the checkStable().
 *	_NG
 */

#ifndef CHECKPRED_H
#define CHECKPRED_H

#include "Thread.h"
#include "ChordNode.h"
#include "ProtocolSingleton.h"

class CheckPred: public Thread {

public:
	CheckPred(ChordNode *n){ node = n; }

	void run() {
		while (1) {
			node->checkPredecessor();
			// wait... timeToCheck millisecond
			for (int i = 0; i < 1000; i++) {
				this->usleep(node->getTimeToCheck());
			}
		}
	}

	ChordNode *node;
};

#endif
