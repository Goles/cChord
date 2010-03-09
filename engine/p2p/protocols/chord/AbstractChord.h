/*
 *  AbstractChord.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/15/10.
 *  Copyright 2010 INRIA - Lognet. All rights reserved.
 *
 *	The original Java Chord class used a nice interface and abstract
 *	class pattern. However interfaces are not very nice in C++... so
 *	this Abstract class is suposed to be used by "subclassing" it 
 *	(inhertance). There the virtual methods must be declared and overriding
 *	some of the Chord Class methods is also a possibility.
 *
 *	_NG
 *
 */

#ifndef ABSTRACTCHORD_H
#define ABSTRACTCHORD_H

#include "Node.h"
#include "ChordTransportCode.h"
#include "IOverlay.h"
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
	void			join(Node* chord);
	void			join(const string &host, int port);
	void			stabilize();
	void			notify(Node* n);
	void			fixFingersTable();

	virtual void	checkStable() = 0;
	void			getObjectOnJoin(Node* n);
	
	/*Abstract Methods*/
	virtual string sendRequest(string message, Node* destination) = 0;
	
	/*Display*/
	string toString();
	
	/*Getters & Setters*/
	Node*	getThisNode() { return thisNode; }
	
	Node*	getSuccessor(){ return successor; }
	void	setSuccessor(Node *n) { successor = n; }
	
	Node*	getPredecessor() { return predecessor; }
	void	setPredecessor(Node* n) { predecessor = n; }

	bool	getAlive() { return alive; }
	void	setAlive(bool a) { alive = a; }
	
	int		getTimeToCheck() { return timeToCheck; }
	void	setTimeToCheck(int t) { timeToCheck = t; }

	/* Helper Methods */
	bool	insideRange(int id, int a, int b);
	void	printStatus();

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
	
	bool alive;
};

#endif
