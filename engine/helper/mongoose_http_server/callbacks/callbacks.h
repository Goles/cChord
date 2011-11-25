/*
 *  callbacks.h
 *  iPhone_POC_P2P
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010

 *
 */

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "AbstractChord.h"
#include "chord_callbacks.h"
#include "http_operations.h"
#include <stdlib.h>

/*
 * This function assigns a reference to the ChordNode to be used by the callbacks
 * in order to communicate the Transport Layer and the P2P node.
 */
void assignNodeReference(AbstractChord *a);

/*
 *	Tracker request handler. This function should handle the tracker response,
 *	this is because the tracker will respond with a POST, which is NOT how the 
 *  peers communicate. (at least for now)
 */
void call_tracker_handler(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

/*
 *	This should be the main webserver callback, and should be able to 
 *	handle all the Chord messages. ( GETPRED, FINDSUCC... etc. ).
 *	It also assumes the message comes in the format "<IP+PORT>,MESSAGE"
 *	(key,value).
 *  
 */
void call_request_handler(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

/*
 *	This functions are mostly for experimental purposes. It should support to send POST 
 *  back, to a peer who sent POST first ( PING - PONG ), with the same message, similar
 *  to "echo".
 */
void call_ping(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_pong(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

/*
 * One of the first test cases, just a simple callback to display the requester's IP
 * on a webpage.
 */
void show_secret(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

#endif
