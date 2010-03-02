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

class MyThread;

using namespace std;

class ChordNode : public AbstractChord
{	
public:
	ChordNode(const string &ip, int port);
	ChordNode(const string &ip, int port, const string &s);
	~ChordNode();
	void initializer(const string &ip, int port, const string &overlayIntifier); //This is to help the default constructor.
	
	void			setIdentifier(const string &iD) { overlayIntifier = iD; }
	void			setTransport(TransportHTTP *t) { transport = t; }
	
	virtual string	forward(string message, Node* destination);
	virtual void	checkStable();
	
	// /////////////////////////////////////////// //
	//      IRequestHandler Pure METHODS           //
	// /////////////////////////////////////////// //
	string handleRequest(const string &code)
	{
		return "";
	}
	
	void kill()
	{
		printf("KILL\n");
	}
	
	// /////////////////////////////////////////// //
	//			IOverlay Pure METHODS              //
	// /////////////////////////////////////////// //
	string			getIdentifier(){ return overlayIntifier; }
	ITransport*		getTransport(){ return transport; }
	Node*			getThisNode(){ return thisNode; }
	
	int keyToH(string key)
	{
		return 0; //TODO: Imeplement keyToH
	}
	
	void put(string key, string value)
	{
		
	}
	
	string get(string key)
	{
		return "";
	}
	
	void join(string host, int port)
	{
		
	}
	
protected:
	TransportHTTP*	transport;
	
private:
	MyThread*		stableThread;
	string			overlayIntifier;
	bool			debugMode;
};

#endif