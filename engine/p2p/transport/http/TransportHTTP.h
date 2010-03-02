/*
 *  TransportHTTP.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/26/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#ifndef TRANSPORT_HTTP_H
#define TRANSPORT_HTTP_H

#include "ITransport.h"
#include <iostream>
#include <string>
#include <sstream>

class ChordNode;

class TransportHTTP: public ITransport
{
public:
	
	//Constructor.
	TransportHTTP(int port /*, IOverlay o*/)
	{
		this->setPort(port);
		/*this-> setOverlay(o);*/
	}
	
	string	sendRequest(const string &message, Node *destination);
	string	sendRequest(string *callback, const string &message, Node *destination); //To override the sendReq pure virtual
	string	sendTrackerRequest(const string &host, int port, const string &callback);
	string	doStuff(const string &code);
	void	test();
	
	//Simple tracker interface.
	string connectToTracker(const string &ip, int port, Node *n);
	
private:
	
	int port;
	//Missing input-ouput overlay.
};

#endif