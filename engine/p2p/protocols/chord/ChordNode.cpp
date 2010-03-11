/*
 *  ChordNode.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/18/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#include "ChordNode.h"
#include "MyThread.h"
#include "tomcrypt.h"
#include "sha1toint.h"
#include "TransportHTTP.h"
#include "callbacks.h"
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>


// /////////////////////////////////////////// //
//           CONSTRUCTOR & DESTRUCTOR          //
// /////////////////////////////////////////// //
ChordNode::ChordNode(const string &ip, int port)
{		
	stringstream ss (stringstream::in | stringstream::out);
	
	//construct the overlayIdentifier.
	ss << "<" << ip << port << ">"; 
	
	//Init the node.
	this->initializer(ip, port, ss.str());
}

ChordNode::ChordNode(const string &ip, int port, const string &s)
{
	this->initializer(ip, port, s);
}

ChordNode::ChordNode(const string &ip, int port, const string &s, unsigned int id)
{
	this->initializer(ip, port, s, id);
}

ChordNode::~ChordNode()
{
	stableThread->kill();
	delete stableThread;
	delete transport;
}

// INITIALIZER WITH SHA-1
void ChordNode::initializer(const string &ip, int port, const string &overlayIntifier)
{
	//Allocate the container for the hash.
	unsigned char out[overlayIntifier.length()];

	//Initialize the SHA-1 hash function according to Tomcrypt library.
	hash_state sha1;
	sha1_init(&sha1);

	//Apply SHA-1 to the overlayIntifier.
	unsigned char *in = (unsigned char*)overlayIntifier.c_str();
	sha1_process(&sha1, in, overlayIntifier.length());
	sha1_done(&sha1, out);

	//Transform the SHA-1 String to an integer.
	unsigned int id = sha1ToInt(out);

	//call default initializer
	this->initializer(ip, port, overlayIntifier, id);
}

void ChordNode::initializer(const string &ip, int port, const string &overlayIntifier, unsigned int id)
{
	//create our stabilizer thread instance.
	stableThread = new MyThread(0, this);
	
	//Initialize the transport layer.
	transport = new TransportHTTP(port);
	
	//set the overlay identifier.
	this->overlayIntifier = overlayIntifier;
	
	//Call our parent's initializer
	initialise(ip, id, port);
	
	// check if the idE[0, 2^(spacesize - 1)]
	assert(!(id >  pow(2, spacesize)));

	//We start-up our stabilizer thread.
	checkStable();
}

// /////////////////////////////////////////// //
//               PUBLIC METHODS                //
// /////////////////////////////////////////// //

/*
 *	Forward a message to a peer, the message is in the format: "<IP+PORT>,TRANSPORT_CODE"
 */
string ChordNode::sendRequest(Request *request, Node* destination)
{
	return transport->sendRequest(request, destination);
}

/*
 *	Starts up the "stabilizer thread" for this peer.
 */
void ChordNode::checkStable()
{
	stableThread->start();
}
