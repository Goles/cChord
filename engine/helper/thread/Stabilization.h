/*
 *  Stabilization.h
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

#ifndef STABILIZATION_H
#define STABILIZATION_H

#include "Thread.h"
#include "ChordNode.h"
#include "ProtocolSingleton.h"

class Stabilization: public Thread {

public:
	Stabilization(ChordNode *n) { node = n; }

	void run() {
		while (1) {
			node->stabilize();
			node->fixFingersTable();
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
