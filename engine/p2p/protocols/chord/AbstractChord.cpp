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

	//Forge the message that we will sendRequest (FINDSUCC)
    std::ostringstream oss;
    oss << id;
    std::string idStr = oss.str();
	Request *request = new Request(this->getIdentifier(), FINDSUCC, idStr);

	string succ = sendRequest(request, pred);
	return new Node(succ);
}

Node* AbstractChord::closestPrecedingNode(int nid)
{
	// optimization
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
	//Forge the message that we will sendRequest (GETPRED)
	Request *request = new Request(this->getIdentifier(), GETPRED, "");
	string pred = sendRequest(request, successor);
	
	if(pred.compare(string("ERROR")) == 0)
	{
		cout << "----------------------------------------------------------------\n";
		cout << "Peer successor unreachable, this should be handled in the future." << endl;
		cout << "----------------------------------------------------------------\n" << endl;
		return;
	}
	   
	if(pred.compare(thisNode->toString()))
	{
		Node *x = new Node(pred);
		if(insideRange(x->getId(), thisNode->getId() + 1, successor->getId() - 1))
		   successor = x;

		//Forge the message that we will sendRequest (NOTIF)
		request = new Request(this->getIdentifier(), NOTIF, thisNode->toString());
		sendRequest(request, successor);
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
