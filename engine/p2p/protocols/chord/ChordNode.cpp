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
#include <sstream>
#include <iostream>
#include "TransportHTTP.h"
#include "callbacks.h"

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

ChordNode::~ChordNode()
{
	stableThread->kill();
	delete stableThread;
	delete transport;
}

/*
 * This is equivalent to the Second Constructor code in ChordNode.java
 * public ChordNode(String ip, int port, String overlayIntifier) {
 */

void ChordNode::initializer(const string &ip, int port, const string &overlayIntifier)
{
	//create our stabilizer thread instance.
	stableThread = new MyThread(0, this);
	
	//Initialize the transport layer.
	transport = new TransportHTTP(80);
	
	//set the overlay identifier.
	this->overlayIntifier = overlayIntifier;
	
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
	
	//Call our parent's initializer
	initialise(ip, id, port);
	
	//We start-up our stabilizer thread.
	checkStable();
}

// /////////////////////////////////////////// //
//               PUBLIC METHODS                //
// /////////////////////////////////////////// //

/*
 *	Forward a message to a peer, the message is in the format: "<IP+PORT>,TRANSPORT_CODE"
 */
string ChordNode::forward(string message, Node* destination)
{
	stringstream ss_message (stringstream::in | stringstream::out);
	
	//form the message "Node_identifier,message" format.
	ss_message << this->getIdentifier() << "," << message;
	
	return transport->sendRequest(ss_message.str(), destination);
}

/*
 *	Starts up the "stabilizer thread" for this peer.
 */
void ChordNode::checkStable()
{
	stableThread->start();
}
