/*
 *  ChordNode.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
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

class MyThread;

using namespace std;

class ChordNode : public AbstractChord
{	
public:
	/* Constructor & Desstructor */
	ChordNode(const string &ip, int port);
	ChordNode(const string &ip, int port, const string &s);
	ChordNode(const string &ip, int port, const string &s, unsigned int id); // To test you can define the id manually
	~ChordNode();
	
	/* Init */
	void initializer(const string &ip, int port, const string &overlayIntifier); 		 			// Initialization which use SHA-1
	void initializer(const string &ip, int port, const string &overlayIntifier, unsigned int id);	// Initialization by default
	
	/* Setters */
	void setIdentifier(const string &iD) { overlayIntifier = iD; }
	void setTransport(TransportHTTP *t)	 { transport = t; }
	
	/* Action Methods */
	virtual string	sendRequest(Request *request, Node* destination);
	virtual void	checkStable();

	string 			printTable();

	/* IOverlay Pure METHODS */
	void			put(string key, string value);
	string			get(string key);
	int				keyToH(string key);
	string			getIdentifier()	{ return overlayIntifier; }
	ITransport*		getTransport()	{ return transport; }
	Node*			getThisNode()	{ return thisNode; }
	
protected:
	TransportHTTP*	transport;
	
private:
	MyThread*		stableThread;
	string			overlayIntifier;
	// Part of the DHT
	typedef std::pair<string, string> data;
	typedef	std::map<string, string> dataMap;
	dataMap table;
};

#endif
