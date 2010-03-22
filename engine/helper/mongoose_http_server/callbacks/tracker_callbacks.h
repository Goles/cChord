/*
 *  tracker_callbacks.h
 *  P2P_Engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *	
 *	This file contains all the callbacks to effectively talk with the tracker using HTTP.
 */

#ifndef TRACKER_CALLBACKS_H
#define TRACKER_CALLBACKS_H

void call_tracker_addnode(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_tracker_getconnection(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_tracker_removenode(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_tracker_join(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

#endif

