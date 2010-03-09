/*
 *  chord_callbacks.cpp
 *  P2P_Engine
 *
 *  Created by Nicolas Goles on 2/9/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "chord_callbacks.h"
#include "ProtocolSingleton.h"
#include "mongoose.h"
#include <assert.h>

/*
 *	/getpred callback, handles a get_predecessor peer request.
 */
void call_chord_getpred(struct mg_connection *conn,
						const struct mg_request_info *request_info,
						void *user_data)
{
	string result;
	
	if(P_SINGLETON->getChordNode() != NULL)
	{
		result = P_SINGLETON->getChordNode()->getPredecessor()->toString();
	}else {
		
		assert(P_SINGLETON->getChordNode() == NULL);
	}
		
	//Display answer as a webpage to the peer requesting predecessor
	mg_printf(conn, "%s", result.c_str());

}

/*
 *	/findsucc callback, handles a find_successor peer request.
 */
void call_chord_findsucc(struct mg_connection *conn,
						 const struct mg_request_info *request_info,
						 void *user_data)
{
	//
}

/*
 *	/notif callback, handles a notification peer request.
 */
void call_chord_notif(struct mg_connection *conn,
					  const struct mg_request_info *request_info,
					  void *user_data)
{
	//
}

/*
 *	/join callback, handles a join chord peer request.
 */
void call_chord_join(struct mg_connection *conn,
					 const struct mg_request_info *request_info,
					 void *user_data)

{
	//
}

/*
 *	/put callback, handles a put peer request.
 */
void call_chord_put(struct mg_connection *conn,
					const struct mg_request_info *request_info,
					void *user_data)
{
	//
}

/*
 *	/get callback, handles a get peer request.
 */
void call_chord_get(struct mg_connection *conn,
					const struct mg_request_info *request_info,
					void *user_data)
{
	//
}

/*
 *	/setsucc callback, handles a set successor peer request.
 */
void call_chord_setsucc(struct mg_connection *conn,
						const struct mg_request_info *request_info,
						void *user_data)
{
	//
}

/*
 *	/setpred callback, handles a set predecessor peer request.
 */
void call_chord_setpred(struct mg_connection *conn,
						const struct mg_request_info *request_info,
						void *user_data)
{
	//
}