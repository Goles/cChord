/*
 *  ProtocolSingleton.cpp
 *  P2P_Engine
 *
 *  Created by Nicolas Goles on 2/9/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ProtocolSingleton.h"

ProtocolSingleton* ProtocolSingleton::instance = NULL;

//Constructor in case you need to init something by default.
ProtocolSingleton::ProtocolSingleton()
{
	myNode = NULL;
}

//Destructor
ProtocolSingleton::~ProtocolSingleton()
{
	delete myNode;
	delete instance;
}

//Singleton creator
ProtocolSingleton* ProtocolSingleton::getInstance()
{
	if(instance == NULL)
		instance = new ProtocolSingleton();
	
	return instance;
}

//Initializes a ChordNode from the given parameters.
void ProtocolSingleton::initChordNode(const string &ip, int port, const string &s)
{
	myNode = new ChordNode(ip, port, s);
}

//Initializes a ChordNode from the given parameters.
void ProtocolSingleton::initChordNode(const string &ip, int port, const string &s, unsigned int nid)
{
	myNode = new ChordNode(ip, port, s, nid);
}

void ProtocolSingleton::stopChord()
{
	delete myNode;
	myNode = NULL;
}