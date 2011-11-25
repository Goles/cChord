/*
 *  AbstractChord.h
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#ifndef ABSTRACTCHORD_H
#define ABSTRACTCHORD_H

#include "Node.h"
#include "ChordTransportCode.h"
#include "IOverlay.h"
#include "Request.h"
#include <vector>
#include <map>

class AbstractChord: public IOverlay
{

public:

	void initialise(string ip, int id, int port);
	
	/*This is the maximum Finger's Table Size*/
	int spacesize;
	
	/* Chord algorithm */
	Node*			findSuccessor(int id);
	Node*			closestPrecedingNode(int id);
	void			stabilize();
	void			notify(Node* n);
	void			fixFingersTable();
	void			join(Node* chord);
	void			checkPredecessor();

	virtual void	checkStable() = 0;
	
	/* Abstract Methods */
	virtual string sendRequest(Request *request, Node* destination) = 0;
	
	/* Display */
	string toString();

	/* Getters & Setters */
	Node*	getThisNode() { return thisNode; }
	
	Node*	getSuccessor(){ return successor; }
	void	setSuccessor(Node *n) { successor = n; }
	
	Node*	getPredecessor() { return predecessor; }
	void	setPredecessor(Node* n) { predecessor = n; }

	int		getTimeToCheck() { return timeToCheck; }
	void	setTimeToCheck(int t) { timeToCheck = t; }

	/* Helper Methods */
	bool	insideRange(int id, int a, int b);
	string	printStatus();

protected:
	typedef map<int, string> stringMap;
	typedef map<int, string>::iterator stringMapIterator;
	typedef vector<Node *> nodesVector;
	
	/** represent this node */
	Node* thisNode;
	
	/** Node predecessor */
	Node* predecessor;
	
	/** Node successor */
	Node* successor;
	
	/** Fingers table */
	nodesVector fingerTable;
	
	/** hash Table containing the keys i am responsible for */
	stringMap table;
	
	/** index of the next finger */
	int next;
	
	/** Time between each stabilization/fix fingerstable */
	int timeToCheck;
};

#endif
