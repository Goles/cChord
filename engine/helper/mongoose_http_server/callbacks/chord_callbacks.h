/*
 *  chord_callbacks.h
 *  P2P_Engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010

 *
 *	This file contains all the callbacks to effectively handle the Chord Protocol
 *	via the HTTP server.
 */

#ifndef CHORD_CALLBACKS_H
#define CHORD_CALLBACKS_H

void call_chord_getpred(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_findsucc(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_notif(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_join(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_put(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_get(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_removekey(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_setsucc(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_setpred(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_hash(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_shutdown(struct mg_connection *conn,
					 const struct mg_request_info *request_info, void *user_data);

#endif
