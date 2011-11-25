/*
 *  ChordNode.h
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
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

using namespace std;

class ChordNode: public AbstractChord {
public:
	/* Constructor & Destructor */
	ChordNode(const string &ip, int port, const string &overlayIdentifier,
			const string &rootDirectory);
	~ChordNode();

	/* Setters */
	void setIdentifier(const string &iD) { overlayIdentifier = iD; }
	void setTransport(TransportHTTP *t)  { transport = t; }

	/* Action Methods */
	virtual string sendRequest(Request *request, Node* destination);
	virtual void fixBrokenPointers(Node* node);
	virtual bool isAlone();
	virtual void checkStable();
	virtual void shutDown();

	/* Override Methods */
	void notify(Node *n);
	void stabilize();

	/* IOverlay Pure METHODS */
	void   put(string key, string value);
	string get(string key);
	void   removekey(string key);


	/* data CRUD */
	void saveData(string filename, string value);
	string openData(string filename);
	string serialize(string data);
	string unserialize(string data);

	/* tools methods */
	unsigned int   getIntSHA1(string key);
	char *getHexSHA1(string str);
	string getIdentifier() 			{ return overlayIdentifier; }
	TransportHTTP* getTransport()   { return transport; }
	Node* getThisNode() 			{ return thisNode; }

	/* the transport layer */
	TransportHTTP* transport;

private:
	/* One thread for the stabilization of each node */
	Stabilization* stableThread;
	/* The id of the overlay */
	string overlayIdentifier;
	/* node notification */
	bool notified;
	/* Part of the DHT */
	typedef std::pair<string, string> data;
	typedef std::map<string, string> dataMap;
	dataMap table;
};

#endif
