/*
 *  ChordNode.cpp
 *  iPhone_p2p_engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

#include "ChordNode.h"
#include "Stabilization.h"
#include "CheckPred.h"
#include "FixFinger.h"
#include "tomcrypt.h"
#include "TransportHTTP.h"
#include "callbacks.h"
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>

/* Constructor */
ChordNode::ChordNode(const string &ip, int port, const string &overlayIdentifier,
		int id, const string &rootDirectory) {

	// Define the address space size
	spacesize = 9;

	// check if the idE[0, 2^(spacesize - 1)]
	assert(!(id > pow(2, spacesize)) && !(id < 0));

	//create our stabilizer threads instance.
	stableThread = new Stabilization(this);
//	fixFingerThread = new FixFinger(this);
//	checkPredThread = new CheckPred(this);

	//Initialize the transport layer.
	transport = new TransportHTTP(port, rootDirectory);

	//set the overlay identifier.
	this->overlayIdentifier = overlayIdentifier;

	// not yet notified
	notified = false;

	//Call our parent's initializer
	initialise(ip, id, port);

	//We start-up our stabilizer thread.
	checkStable();
}

/* Destructor */
ChordNode::~ChordNode() {
	stableThread->kill();
	delete stableThread;
	delete transport;
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
	int hKey = getMD5(key);
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
	int hKey = getMD5(key);
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
/* DHT Remove */
void ChordNode::removekey(string key) {
	// Convert the key in a hash integer
	int hKey = getMD5(key);
	if (insideRange(hKey, predecessor->getId() + 1, thisNode->getId())) {
		// I'm responsible for this key
		dataMap::iterator it = table.find(key);
		if (it != table.end()) {
			table.erase(it);
		}
	} else {
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), REMOVEKEY);
		request->addArg("key", key);
		// Send the Put request
		sendRequest(request, responsible);
	}
}

/* Convert a string to an integer (using MD5) */
unsigned int ChordNode::getMD5(string str) {
	//	// The integer value depend of the overlayIdentifier
	//	str.append(overlayIdentifier.c_str());
	//
	//	hash_state md;
	//	unsigned char md5[16];
	//
	//	//Initialize the MD5 hash function according to Tomcrypt library.
	//	md5_init(&md);
	//
	//	//Apply SHA-1 to the input string.
	//	md5_process(&md, (unsigned char *) str.c_str(), str.length());
	//
	//	//Get the hash output.
	//	md5_done(&md, md5);
	//
	//	// transform the md5 string to an integer
	//	unsigned int md5toInt = 0;
	//	for (int i = 0; i < strlen((const char *) md5) + 1; i++)
	//		md5toInt = md5toInt * 256 + (md5[i] & 0xff);
	//
	//	// check if the idE[0, 2^(spacesize - 1)]
	//	assert(!(md5toInt > pow(2, spacesize)));
	//
	//	return md5toInt;

	int md5 = atoi(str.c_str());
	assert(!(md5 > pow(2, spacesize)) && !(md5 < 0));
	return md5;
}

/* Forward a message to a peer, the message is in the format: "<IP+PORT>,TRANSPORT_CODE" */
string ChordNode::sendRequest(Request *request, Node* destination) {
	char *response = transport->sendRequest(request, destination);
	if (response) {
		stringstream ss;
		ss << response;
		free(response); // we must free the initial char* response, to avoid leaks.
		return ss.str();
	} else {
		// Fix the broken pointers of the node
		fixBrokenPointers(destination);
		// time to fix the chord
		sleep(1);
		// try again the request with a new destination
		return sendRequest(request, findSuccessor(destination->getId()));
	}
}

void ChordNode::fixBrokenPointers(Node *node) {
	for (int i = 0; i < fingerTable.size() - 1; i++) {
		if (fingerTable[i]->getId() == node->getId()) {
			fingerTable[i] = new Node(thisNode->toString());
		}
	}
	if (predecessor->getId() == node->getId()) {
		predecessor = new Node(thisNode->toString());
	}
	if (successor->getId() == node->getId()) {
		successor = new Node(thisNode->toString());
	}
	if(predecessor->getId() == thisNode->getId() &&
	   thisNode->getId() == successor->getId()){
		cout << "I lost the chord... \n";
	}
}

/* Starts up the "stabilizer thread" for this peer. */
void ChordNode::checkStable() {
	stableThread->start();
//	fixFingerThread->start();
//	checkPredThread->start();
}

/* Stop the stabilization, distribute the key and shutDown the peer */
void ChordNode::shutDown() {
	// kill the stabilization Threads
	stableThread->kill();
//	fixFingerThread->kill();
//	checkPredThread->kill();


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
