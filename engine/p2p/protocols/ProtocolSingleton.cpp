/*
 *  ProtocolSingleton.cpp
 *  P2P_Engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#include "ProtocolSingleton.h"

ProtocolSingleton* ProtocolSingleton::instance = NULL;

//Constructor in case you need to init something by default.
ProtocolSingleton::ProtocolSingleton() {
	myNode = NULL;
}

//Destructor
ProtocolSingleton::~ProtocolSingleton() {
	delete myNode;
	delete instance;
}

//Singleton creator
ProtocolSingleton* ProtocolSingleton::getInstance() {
	if (instance == NULL)
		instance = new ProtocolSingleton();

	return instance;
}

//Initializes a ChordNode from the given parameters.
ChordNode* ProtocolSingleton::initChordNode(const string &ip, int port,
		const string &overlayIntifier,
		const string &rootDirectory) {
	myNode = new ChordNode(ip, port, overlayIntifier, rootDirectory);

	return myNode;
}

void ProtocolSingleton::stopChord() {
	delete myNode;
	myNode = NULL;
}
