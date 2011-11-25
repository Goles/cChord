/*
 *  callbacks.c
 *  iPhone_POC_P2P
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010

 *
 */

#include "callbacks.h"

#include <assert.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include "mongoose.h"
#include "ChordTransportCode.h"
#include "ProtocolSingleton.h"

/*
 *	Tracker request handler. This function should handle the tracker response,
 *	this is because the tracker will respond with a POST, which is NOT how the 
 *  peers communicate. (at least for now)
 */
void call_tracker_handler(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	//HERE I MUST EXTRACT THE POST MESSAGE OF THE TRACKER! (maybe we will need to handle that in base64).
}

/*
 *	This should be the main webserver callback, and should be able to 
 *	handle all the Chord messages. ( GETPRED, FINDSUCC... etc. ).
 *	It also assumes the message comes in the format "<IP+PORT>,MESSAGE"
 *	(key,value).
 *  
 */
void call_request_handler(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	char* auxMessage = (char *) malloc(sizeof(strlen(request_info->post_data)));
	int message = -1;
	string result;

	if (auxMessage != NULL) {
		strcpy(auxMessage, request_info->post_data);
		message = getPostMessage(auxMessage);
		free(auxMessage);
	}

	printf("\n**HANDLING PEER REQUEST**\n");

	switch (message) {
	case GETPRED:
		printf("** GETPRED **\n");
		if (P_SINGLETON->getChordNode() != NULL) {
			result = P_SINGLETON->getChordNode()->getPredecessor()->toString();
		}
		break;
	case FINDSUCC:
		printf("** FINDSUCC **\n");
		break;
	case NOTIF:
		printf("** NOTIF **\n");
		break;
	case JOIN:
		printf("** JOIN **\n");
		break;
	case PUT:
		printf("** PUT **\n");
		break;
	case GET:
		printf("** GET **\n");
		break;
	case SETSUCC:
		printf("** SETSUCC **\n");
		break;
	case SETPRED:
		printf("** SETPRED **\n");
		break;
	default:
		printf("** UNHANDLED PEER REQUEST! **\n");
		assert(message);
		break;
	}

	mg_printf(conn, "%s", result.c_str());
}

/*
 *	This functions are mostly for experimental purposes. It should support to send POST 
 *  back, to a peer who sent POST first ( PING - PONG ), with the same message, similar
 *  to "echo".
 */
void call_ping(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "%s", "Content-Type: text/html\r\n\r\n");
	mg_printf(conn, "%s", "<html><body>");
	mg_printf(conn, "%s", "<p>Been \"PING\" sending \"PONG\" ");
	mg_printf(conn, "%s %s", "to ", returnIP(conn));
	mg_printf(conn, "%s", "</body></html>");

	printf("Been PING...sending PONG...\n");

	sendPost((char *) returnIP(conn), 8080, (char *) "/pong",
			request_info->post_data);
}

void call_pong(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "%s", "Content-Type: text/html\r\n\r\n");
	mg_printf(conn, "%s", "<html><body>");
	mg_printf(conn, "%s", "<p>\"PONG\" sent from ");
	mg_printf(conn, "%s", returnIP(conn));
	mg_printf(conn, "%s", "<br>");
	mg_printf(conn, "%s %s", "Message: ", request_info->post_data);
	mg_printf(conn, "%s", "</body></html>");

	printf("...PONG...\n");

	sendPost((char *) returnIP(conn), 8080, (char *) "/test",
			(char *) "PONG!!!");
}

/*
 * One of the first test cases, just a simple callback to display the requester's IP
 * on a webpage.
 */
void show_secret(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "%s", "Content-Type: text/html\r\n\r\n");
	mg_printf(conn, "%s", "<html><body>");
	mg_printf(conn, "%s", "<p>PING</body></html>");

	//struct in_addr address_struct;
	//address_struct.s_addr = request_info->remote_ip + 1;

	mg_printf(conn, "%s", "<br>");
	mg_printf(conn, "%s", "You are ");
	mg_printf(conn, "%s", returnIP(conn));
}
