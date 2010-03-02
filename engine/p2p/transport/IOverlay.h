/*
 *  IOverlay.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *	Abstract Class IOverlay inherits from the ITransport Class... trying to simulate
 *	an interface.
 */

#ifndef IOVERLAY_H
#define IOVERLAY_H

#include "IRequestHandler.h"
#include "Node.h"
#include "ITransport.h"
#include <string>

class IOverlay: public IRequestHandler
{
public:
	virtual string		getIdentifier()			= 0;
	virtual int			keyToH(string key)		= 0;
	virtual ITransport* getTransport()			= 0;
	virtual Node*		getThisNode()			= 0;
	
	virtual void	put(string key, string value)	= 0;
	virtual string	get(string key)					= 0;
	virtual void	join(string host, int port)		= 0;
};


#endif