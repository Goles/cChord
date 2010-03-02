/*
 *  ITransport.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/26/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

/*
 * This is an abstract class with pure virtual methods.
 */

#ifndef ITRANSPORT_H
#define ITRANSPORT_H

#include <string>
#include "Node.h"

class ITransport
{
public:
	virtual string	sendRequest(const string &message, Node *destination) = 0;
	virtual	string	connectToTracker(const string &ip, int port, Node *n) = 0;
	int				getPort() { return port; }
	void			setPort( int p ) { port = p; }
		
protected:
	int port;
};

#endif