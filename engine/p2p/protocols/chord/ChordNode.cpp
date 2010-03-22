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

/* Constructor & Destructor */
ChordNode::ChordNode(const string &ip, int port) {
	stringstream ss(stringstream::in | stringstream::out);

	//construct the overlayIdentifier.
	ss << "<" << ip << port << ">";

	//Init the node.
	this->initializer(ip, port, ss.str());
}

ChordNode::ChordNode(const string &ip, int port, const string &s) {
	this->initializer(ip, port, s);
}

ChordNode::ChordNode(const string &ip, int port, const string &s,
		unsigned int id) {
	this->initializer(ip, port, s, id);
}

ChordNode::~ChordNode() {
	stableThread->kill();
	delete stableThread;
	delete transport;
}

/* Init */
void ChordNode::initializer(const string &ip, int port,
		const string &overlayIntifier) {
	//Allocate the container for the hash.
	unsigned char out[overlayIntifier.length()];

	//Initialize the SHA-1 hash function according to Tomcrypt library.
	hash_state sha1;
	sha1_init(&sha1);

	//Apply SHA-1 to the overlayIntifier.
	unsigned char *in = (unsigned char*) overlayIntifier.c_str();
	sha1_process(&sha1, in, overlayIntifier.length());
	sha1_done(&sha1, out);

	//Transform the SHA-1 String to an integer.
	unsigned int id = sha1ToInt(out);

	//call default initializer
	this->initializer(ip, port, overlayIntifier, id);
}

void ChordNode::initializer(const string &ip, int port,
		const string &overlayIntifier, unsigned int id) {
	//create our stabilizer thread instance.
	stableThread = new MyThread(0, this);

	//Initialize the transport layer.
	transport = new TransportHTTP(port);

	//set the overlay identifier.
	this->overlayIntifier = overlayIntifier;

	// not yet notified
	notified = false;

	//Call our parent's initializer
	initialise(ip, id, port);

	// check if the idE[0, 2^(spacesize - 1)]
	assert(!(id > pow(2, spacesize)));

	//We start-up our stabilizer thread.
	checkStable();
}

/* Override from AbstractChord */
void ChordNode::notify(Node *n) {
	Node *pred = predecessor;
	((AbstractChord *) this)->notify(n);
	// If the predecessor as changed, update the DHT table
	if (pred != predecessor) {
		notified = true;
	}
}

void ChordNode::stabilize() {
	((AbstractChord *) this)->stabilize();
	// If the predecessor as changed, update the DHT table
	if (notified) {
		for (dataMap::iterator it = table.begin(); it != table.end(); ++it) {
			Request *request = new Request(this->getIdentifier(), PUT);
			int id = atoi(it->first.c_str());
			if (!insideRange(id, predecessor->getId(), thisNode->getId())) {
				request->addArg("key", it->first);
				request->addArg("value", it->second);
				// Send the Put request
				sendRequest(request, predecessor);
				// remove the key from my table
				table.erase(it);
			}
		}
		notified = false;
	}
}

/* DHT Put */
void ChordNode::put(string key, string value) {
	// Convert the key in a hash integer
	int hKey = keyToH(key);
	if (insideRange(hKey, predecessor->getId() + 1, thisNode->getId())) {
		// I'm responsible for this key
		table.insert(data(key, value));
	} else {
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), PUT);
		request->addArg("key", key);
		request->addArg("value", value);
		// Send the Put request
		sendRequest(request, responsible);
	}
}

/* DHT Get */
string ChordNode::get(string key) {
	// Convert the key in a hash integer
	int hKey = keyToH(key);
	if (insideRange(hKey, predecessor->getId() + 1, thisNode->getId())) {
		// I'm responsible for this key
		dataMap::iterator it = table.find(key);
		if (it != table.end()) {
			return (it->second);
		} else {
			string err = "no result";
			return err;
		}
	} else {
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), GET);
		request->addArg("key", key);
		// Send the Put request
		return sendRequest(request, responsible);
	}
}

/* Convert a string key to an integer (using hash function) */
int ChordNode::keyToH(string key) {
	// HASH FUNCTION IS NOT IMPLEMENTED YET...
	return atoi(key.c_str());
}

/* Forward a message to a peer, the message is in the format: "<IP+PORT>,TRANSPORT_CODE" */
string ChordNode::sendRequest(Request *request, Node* destination) {
	// CheckPredecessor use GETPRED request to check
	// if the predecessor is still alive
	if (request->getCode() == CHECKPRED) {
		request->setCode(GETPRED);
		if (!transport->sendRequest(request, destination)) {
			predecessor = thisNode;
		}
		return "";
	} else {
		char *response = transport->sendRequest(request, destination);
		if (response) {
			stringstream ss;
			ss << response;
			free(response); // we must free the initial char* response, to avoid leaks.
			return ss.str();
		} else {
			// re-build the broken FingerTable and try to send again the request
			fixBrokenFingersTable(destination);
			// time to fix the chord
			sleep(1);
			// try again the request with a new destination
			return sendRequest(request, findSuccessor(destination->getId()));
		}
	}
}

/* Starts up the "stabilizer thread" for this peer. */
void ChordNode::checkStable() {
	stableThread->start();
}

/* Stop the stabilization, distribute the key and shutDown the peer */
void ChordNode::shutDown() {
	stableThread->kill();
	// notify predecessor
	Request *request = new Request(this->getIdentifier(), SETSUCC);
	request->addArg("successor", successor->toString());
	sendRequest(request, predecessor);
	// notify successor
	request = new Request(this->getIdentifier(), SETPRED);
	request->addArg("predecessor", predecessor->toString());
	sendRequest(request, successor);
	// give the part of the DHT to the successor
	for (dataMap::iterator it = table.begin(); it != table.end(); ++it) {
		request = new Request(this->getIdentifier(), PUT);
		request->addArg("key", it->first);
		request->addArg("value", it->second);
		sendRequest(request, successor);
		// remove the key from my table
		table.erase(it);
	}
	cout << "bye bye...\n";
	sleep(1);
	exit(0);
}

/* print node status */
string ChordNode::printTable() {
	stringstream ss(stringstream::in | stringstream::out);
	ss << "data:\n";
	for (dataMap::const_iterator it = table.begin(); it != table.end(); ++it) {
		ss << "\t[" << it->first << "]";
		ss << " - " << it->second << '\n';
	}
	return ss.str();
}
