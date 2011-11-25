/*
 *  FixFinger.h
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

#ifndef FIXFINGER_H
#define FIXFINGER_H

#include "Thread.h"
#include "ChordNode.h"
#include "ProtocolSingleton.h"

class FixFinger: public Thread {

public:
	FixFinger(ChordNode *n){ node = n; }

	void run() {
		while (1) {
			node->fixFingersTable();
			// wait... timeToCheck millisecond
			for (int i = 0; i < 1000; i++) {
				this->usleep(node->getTimeToCheck());
			}
		}
	}

	ChordNode *node;
};

#endif
