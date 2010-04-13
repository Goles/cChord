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
#include "TransportHTTP.h"
#include "callbacks.h"
#include "sha1.h";
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>

/* Constructor */
ChordNode::ChordNode(const string &ip, int port, const string &overlayIdentifier, const string &rootDirectory) {

	// Define the address space size
	spacesize = 9;

	// Create the id
	std::ostringstream oss;
	oss << ip << port;
	int id = getSHA1(oss.str());

	// check if the idE[0, 2^(spacesize - 1)]
	assert(!(id > pow(2, spacesize)) && !(id < 0));

	//create our stabilizer threads instance.
	stableThread = new Stabilization(this);

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
	if (notified && predecessor->getId() != thisNode->getId()) {
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
	int hKey = getSHA1(key);
	if (insideRange(hKey, predecessor->getId() + 1, thisNode->getId())) {
		// I'm responsible for this key
		stringstream ss;
		ss << table[key] << "*******";
		table[key]=ss.str();
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
	int hKey = getSHA1(key);
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
	int hKey = getSHA1(key);
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
unsigned int ChordNode::getSHA1(string str) {
	SHA1 *sha1 = new SHA1();
	sha1->addBytes( str.c_str(), strlen( str.c_str() ));
	unsigned char* digest = sha1->getDigest();
	unsigned int res = sha1->shaToInteger(digest, 20, pow(2, spacesize));
	delete sha1;
	free( digest );
	return res;
}

/* Forward a message to a peer, the message is in the format: "<IP+PORT>,TRANSPORT_CODE" */
string ChordNode::sendRequest(Request *request, Node* destination) {
	char *response = transport->sendRequest(request, destination);
	// response received
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
		// The node is completely disconnected of the backbone
		if (isAlone()) { // there is only one response possible
			return getThisNode()->toString();
		}
		// try again the request with a new destination
		return sendRequest(request, findSuccessor(destination->getId()));
	}
}

/* Fix broken pointers algorithm */
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
}

/* return true if the node is completely disconnected of the chord */
bool ChordNode::isAlone(){
	for (int i = 0; i < fingerTable.size() - 1; i++) {
		if (fingerTable[i]->getId() != thisNode->getId()){
			return false;
		}
	}
	return predecessor->getId() == thisNode->getId() && successor->getId() == thisNode->getId();
}

/* Starts up the "stabilizer thread" for this peer. */
void ChordNode::checkStable() {
	stableThread->start();
}

/* Stop the stabilization, distribute the key and shutDown the peer */
void ChordNode::shutDown() {
	// kill the stabilization Threads
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
