/*
 *  Node.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/14/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#include "Node.h"
#include <iostream>
#include <sstream>

/*
 *	Constructors
 */
Node::Node(const string &inIp, unsigned int inId, int inPort)
{
	ip		= inIp;
	nid		= inId;
	port	= inPort;
}

Node::Node(const string &inIp, int inPort)
{
	ip		= inIp;
	nid		= 0;
	port	= inPort;
}

Node::Node(const string &inData)
{
	stringstream ss(inData);
	stringstream ipStream (stringstream::in | stringstream::out);
	
	int ipa,
		ipb,
		ipc,
		ipd;
	
	char ch;
	
	int inID;
	int inPort;
	
	ss >> ipa >> ch >> ipb >> ch >> ipc >> ch >> ipd >> ch >> inID >> ch >> inPort;
	
	ipStream << ipa << "." << ipb << "." << ipc << "." << ipd;
	
	string ip(ipStream.str());
}

/*
 *	Action Methods
 */

bool Node::equals(Node *aNode)
{
	if(aNode != NULL)
		return((aNode->getId() == nid) && 
			   (aNode->getPort() == port) && 
			   (aNode->getIp() == ip));
	
	return false;
}

string Node::toString()
{
	stringstream aStream (stringstream::in | stringstream::out);
	
	aStream << ip << "," << nid << "," << port;
	
	return(aStream.str());
}
