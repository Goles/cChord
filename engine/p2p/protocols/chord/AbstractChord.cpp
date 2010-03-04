/*
 *  Chord.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AbstractChord.h"
#include <math.h>
#include <iostream>
#include <sstream>

// /////////////////////////////////////////// //
//                  INITIALISE                 //
// /////////////////////////////////////////// //
void AbstractChord::initialise(string ip, int id, int port)
{
	thisNode	= new Node(ip, id, port);
	successor	= thisNode;
	predecessor = thisNode;
	next		= 0; // C++ we have to set next to zero to avoid possible garbage...
	alive		= true;
	spacesize	= 10;
	timeToCheck	= 1;

	for(int i = 0; i < spacesize; i++)
	{
		fingerTable.push_back(thisNode);
	}
}

// /////////////////////////////////////////// //
//               CHORD ALGORITHM               //
// /////////////////////////////////////////// //
Node* AbstractChord::findSuccessor(int id)
{
	if(insideRange(id, thisNode->getId() + 1, successor->getId()))
		return successor;

	Node *pred = closestPrecedingNode(id);

	/*Craft String*/
	stringstream ss (stringstream::in | stringstream::out);
	ss << FINDSUCC << "," << id;

	string succ = forward(ss.str(), pred);
	return new Node(succ);
}

Node* AbstractChord::closestPrecedingNode(int nid)
{
	// optimisation
	if(thisNode == successor) {
		return thisNode;
	}
	for (int i = fingerTable.size() - 1; i > 1; i--)
	{
		if(insideRange(fingerTable[i]->getId(), thisNode->getId() + 1, nid -1)){
			return fingerTable[i];
		}
	}
	return successor;
}

void AbstractChord::stabilize()
{
	//Forge the message that we will forward (GETPRED)
	stringstream ss (stringstream::in | stringstream::out);
	ss << GETPRED;

	string pred = forward(ss.str(), successor);

	if(pred.compare(thisNode->toString()))
	{
		Node *x = new Node(pred);
		if(insideRange(x->getId(), thisNode->getId() + 1, successor->getId() - 1))
		   successor = x;

		stringstream ss1 (stringstream::in | stringstream::out);

		ss1 << NOTIF << "," << thisNode;

		forward(ss1.str(), successor); //TODO: Ask About this. 
	}
}

void AbstractChord::fixFingersTable()
{
	next++;

	if (next > (spacesize - 1))
		next = 1;

	fingerTable[next] = findSuccessor((thisNode->getId() + (int) pow(2, next - 1)) % (int) pow(2, spacesize - 1));
}


// /////////////////////////////////////////// //
//               HELPER METHODS                //
// /////////////////////////////////////////// //
bool AbstractChord::insideRange(int id, int begin, int end)
{
	int MAXid = pow(2, spacesize - 1);
	int MINid = 0;

	return	(begin < end && begin <= id && id <= end) ||
			(begin > end && ((begin <= id && id <= MAXid) ||
			(MINid <= id && id <= end))) ||
			((begin == end) && (id == begin));

}

void AbstractChord::printStatus()
{
	cout << getIdentifier() << " on " << thisNode->getIp() << ":" << thisNode->getPort() << "\n" <<
			"<NODE: " << thisNode->getId() << ", PRED: " << predecessor->getId() << ", SUCC: " << successor->getId() << ">\n" <<
			"\tFingers Table: [";
	for (int i = 0 ; i < fingerTable.size() - 2; i++) {
		cout << fingerTable[i]->getId() << ", ";
	}
	cout << fingerTable[fingerTable.size()-1]->getId() << "]\n\n";
}
