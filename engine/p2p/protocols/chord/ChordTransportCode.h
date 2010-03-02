/*
 *  ChordTransportCode.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 2/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHORD_TRANSPORT_CODE_H
#define CHORD_TRANSPORT_CODE_H

typedef enum
{
	GETPRED = 0,	//GETPRED
	FINDSUCC,		//FINDSUCC,<id>
	NOTIF,			//NOTIF,<ip>,<id>,<port>
	JOIN,
	PUT,
	GET,
	SETSUCC,
	SETPRED
} transportCode;

#endif