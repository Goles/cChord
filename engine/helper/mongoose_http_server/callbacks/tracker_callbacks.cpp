/*
 *  tracker_callbacks.cpp
 *  P2P_Engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

#include "tracker_callbacks.h"
#include "ProtocolSingleton.h"

void call_tracker_addnode(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	//Handle callback here.
}

void call_tracker_getconnection(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	//Handle callback here.
}

void call_tracker_removenode(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	//Handle callback here.
}

void call_tracker_join(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	//Handle callback here.
}
