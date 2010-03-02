/*
 *  tracker_callbacks.cpp
 *  P2P_Engine
 *
 *  Created by Nicolas Goles on 2/9/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#include "tracker_callbacks.h"
#include "ProtocolSingleton.h"

void call_tracker_addnode(struct mg_connection *conn,
						  const struct mg_request_info *request_info,
						  void *user_data)
{
	//Handle callback here.
}

void call_tracker_getconnection(struct mg_connection *conn,
								const struct mg_request_info *request_info,
								void *user_data)
{
	//Handle callback here.
}

void call_tracker_removenode(struct mg_connection *conn,
							 const struct mg_request_info *request_info,
							 void *user_data)
{
	//Handle callback here.
}

void call_tracker_join(struct mg_connection *conn,
					   const struct mg_request_info *request_info,
					   void *user_data)
{
	//Handle callback here.
}