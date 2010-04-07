/*
 *  ChordNode.h
 *  iPhone_p2p_engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 *	This class is the implementation of the "Chord" class,
 *	it inherits from the Chord abstract class and implements
 *	it's abstract methods.
 */

#ifndef CHORDNODE_H
#define CHORDNODE_H

#include "IOverlay.h"
#include "IRequestHandler.h"
#include "AbstractChord.h"
#include "TransportHTTP.h"
#include <string>
#include <map>

class Stabilization;
//class CheckPred;
//class FixFinger;

using namespace std;

class ChordNode: public AbstractChord {
public:
	/* Constructor & Destructor */
	ChordNode(const string &ip, int port, const string &overlayIdentifier, int id, const string &rootDirectory);
	~ChordNode();

	/* Setters */
	void setIdentifier(const string &iD) {
		overlayIdentifier = iD;
	}
	void setTransport(TransportHTTP *t) {
		transport = t;
	}

	/* Action Methods */
	virtual string sendRequest(Request *request, Node* destination);
	virtual void   fixBrokenPointers(Node* node);
	virtual void checkStable();
	virtual void shutDown();

	/* Override Methods */
	void notify(Node *n);
	void stabilize();

	/* IOverlay Pure METHODS */
	void put(string key, string value);
	string get(string key);
	void removekey(string key);
	unsigned int getSHA1(string key);
	
	string		getIdentifier() { return overlayIdentifier; }
	ITransport* getTransport()	{ return transport; }
	Node*		getThisNode()	{ return thisNode; }

	/* Helper Methosha1 = new SHA1();ds */
	string printTable();

protected:
	TransportHTTP* transport;

private:
	Stabilization* stableThread;
	string overlayIdentifier;
	bool notified;
	// Part of the DHT
	typedef std::pair<string, string> data;
	typedef std::map<string, string> dataMap;
	dataMap table;
};

#endif
