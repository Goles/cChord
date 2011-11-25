/*
 *  Node.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#include "Node.h"
#include "Request.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *	Constructors
 */
Node::Node(const string &inIp, unsigned int inId, unsigned int inPort) {
	ip = inIp;
	nid = inId;
	port = inPort;

	//Transform the nid into a string for later usage.
	char nidBuffer[255];
	sprintf(nidBuffer, "%d", nid);
	nidS = string(nidBuffer);
}

Node::Node(const string &inIp, unsigned int inPort) {
	ip = inIp;
	nid = 0;
	port = inPort;

	//Transform the nid into a string for later usage.
	char nidBuffer[255];
	sprintf(nidBuffer, "%d", nid);
	nidS = string(nidBuffer);
}

Node::Node(const string &inData) {
	string inID, inIP, inPort;
	int at = inData.find("@");
	inID = inData.substr(0, at);
	int ddot = inData.find(":");
	inIP = inData.substr(at+1, ddot-at-1);
	inPort = inData.substr(ddot+1);

//	cout << "\n***inID=" << inID << endl;
//	cout << "***inIP=" << inIP << endl;
//	cout << "***inPort=" << inPort << endl;

	this->nidS = inID;
	this->nid = atoi(inID.c_str());
	this->ip = inIP;
	this->port = atoi(inPort.c_str());
}

/*
 *	Action Methods
 */
bool Node::equals(Node *aNode) {
	if (aNode != NULL)
		return ((aNode->getId() == nid) && (aNode->getPort() == port)
				&& (aNode->getIp() == ip));

	return false;
}

/* format: <ID>@<IP>:<PORT> */
string Node::toString() {
	ostringstream ss;
	char portBuffer[255];
	sprintf(portBuffer, "%d", port);
	string portS(portBuffer);
	ss << nidS << "@" << ip << ":" << portS;
	return (ss.str());
}

