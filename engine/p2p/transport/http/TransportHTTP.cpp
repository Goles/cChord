/*
 *  TransportHTTP.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/26/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#include "http_operations.h" //I include .c file for the linker to notice the .o

#include "TransportHTTP.h"
#include "HTTP_Client.h"
#include "ChordNode.h"
#include "IOverlay.h"
#include "callbacks.h"
#include "Request.h"
#include <arpa/inet.h>
#include <sstream>
#include <iostream>

/*
 * Constructor & Destructor.
 */
TransportHTTP::TransportHTTP(int port)
{
	//Constructor.
	this->setPort(port);
	myPort = port;
	
	//Initialize the webserver
	this->startHTTP();
}

TransportHTTP::~TransportHTTP()
{
	this->stopHTTP();
}

/*
 *	HTTP server operations.
 */

//Starts up the mongoose HTTP server.
void TransportHTTP::startHTTP()
{
	char buffer[255]; 
	sprintf(buffer, "%d", myPort);
	
	string a(buffer);
	
	this->ctx = mg_start();     // Start Mongoose serving context thread
	mg_set_option(ctx, "root", ".");  // Set document root
	mg_set_option(ctx, "ports", a.c_str());    // Listen on port XXXX
	this->initCallbacks();
	
	/* Now Mongoose is up, running and configured.
	   Serve until somebody terminates us */
	
	cout << "----------------------------------------------------" << endl;
	cout << "Mongoose Server is running on http://localhost:" << this->getPort() << endl;
	cout << "----------------------------------------------------" << endl;
}

//Stops the mongoose HTTP server.
void TransportHTTP::stopHTTP()
{
	mg_stop(ctx);
	cout << "Mongoose Server is now stopped" << endl;
}

void TransportHTTP::initCallbacks()
{
	if(ctx != NULL)
	{
		/*Test callbacks*/
		mg_set_uri_callback(ctx, "/requesthandler", &call_request_handler, NULL);
		mg_set_uri_callback(ctx, "/ping", &call_ping, NULL);
		mg_set_uri_callback(ctx, "/pong", &call_pong, NULL);
		
		/*Set tracker transport code Callbacks*/
		mg_set_uri_callback(ctx, "/tracker/addnode", &call_tracker_addnode, NULL);
		mg_set_uri_callback(ctx, "/tracker/getconnection", &call_tracker_getconnection, NULL);
		mg_set_uri_callback(ctx, "/tracker/removenode", &call_tracker_removenode, NULL);	
		mg_set_uri_callback(ctx, "/tracker/join", &call_tracker_join, NULL);	
		
		/*Set Chord transport code Callbacks*/
		mg_set_uri_callback(ctx, "/getpred", &call_chord_getpred, NULL);
		mg_set_uri_callback(ctx, "/findsucc", &call_chord_findsucc, NULL);
		mg_set_uri_callback(ctx, "/notif", &call_chord_notif, NULL);
		mg_set_uri_callback(ctx, "/join", &call_chord_join, NULL);
		mg_set_uri_callback(ctx, "/put", &call_chord_put, NULL);
		mg_set_uri_callback(ctx, "/get", &call_chord_get, NULL);
		mg_set_uri_callback(ctx, "/setsucc", &call_chord_setsucc, NULL);
		mg_set_uri_callback(ctx, "/setpred", &call_chord_setpred, NULL);
	}
}


/*
 * Method that encapsulates a simple tracker connection using HTTP.
 */
string TransportHTTP::connectToTracker(const string &ip, int port, Node *n)
{
//	IOverlay	*overlay	= new ChordNode(n->getIp(),n->getPort());
//	Node		*tracker	= new Node(ip,port);
//	stringstream addnode	(stringstream::in | stringstream::out);
//	stringstream conn		(stringstream::in | stringstream::out);
//	
//	string *callback = new string("/tracker/httpHandler.jsp?request=2,student"); //this is freed in sendRequest
//	
//	//string trackerResponse = overlay->getTransport()->sendRequest(conn.str(), tracker);
//	
//	string trackerResponse = this->sendRequest(callback, "", tracker);
//	
//	cout << "TRACKER RESPONSE!! : " << trackerResponse << endl;
//	
//
//	delete callback;
	

	this->sendTrackerRequest("209.85.227.104", 80, "/");
	
	//http_get("209.85.227.99", 80, "/");
}

/*
 * Abstract member function Implementation.
 */
string TransportHTTP::sendRequest(Request *request, Node *destination)
{	
	string *callback;
	
	//We need to forge the corresponding callback string for the message passed.
	switch (request->getCode()) {
		case GETPRED:
			callback = new string("/getpred");
			break;
			
		case FINDSUCC:
			callback = new string("/findsucc");
			break;
			
		case NOTIF:
			callback = new string("/notif");
			break;
		
		case JOIN:
			callback = new string("/join");
			break;
		
		case PUT:
			callback = new string("/put");
			break;
		
		case GET:
			callback = new string("/get");
			break;

		case SETSUCC:
			callback = new string("/setsucc");
			break;
			
		case SETPRED:
			callback = new string("/setpred");
			break;
			
		default:
			cout << " UNHANDLED CHORD TRANSPORT CODE! ... ASSERTING" << endl;
			assert(request->getCode());
			break;
	}
	
	return(this->sendRequest(callback, request, destination));
}

/*
 *	This function is here to send message to other callbacks of the webserver (in case there are).
 */
string TransportHTTP::sendRequest(string *callback, Request *request, Node *destination)
{		
//	cout << "\nMessage: " << message << endl;
//	cout << "Callback: " << *callback << endl;
//	cout << "Destination: " << destination->toString() << endl;
	
	char *response = NULL;
	
	/*
	 *	We should send our POST to the destination, their callback function,
	 *	must manage it accordingly, and the callback that we choose must be passed
	 *	via the callback argument of this function.
	 */
	
	response = sendPost((char *)(destination->getIp()).c_str(), 
						destination->getPort(), 
						(char *)callback->c_str(), 
						(char *)request->serialize().c_str());
	
	//if we have null response from sendPost()
	if(strcmp(response, "ERROR"))
	{
		return "ERROR";
	}
	
	//else
	//string cpp_response = string(response);
	
	stringstream ss;
	
	ss << response;
	
	cout << ss.str() << endl;
	
	//delete callback;
	free(response); // we must free the initial char* response, to avoid leaks.
	return ss.str();
}

/*
 *	Since the tracker can only present HTML responses (for now) and not tweak
 *	the response like mongoose can ( and respond only what we need ). We should
 *  use the following function to send the request in GET format and to receive
 *  only the HTML we want ( and not the headers, etc ).
 */
string TransportHTTP::sendTrackerRequest(const string &host, int port, const string &callback)
{
	//We can't directly pass const values to http_get()	
	char *cHost = NULL;
	char *cCallback = NULL;
	
	//put the strings in their corresponding places.
	cHost = new char [host.size() + 1];
	cCallback = new char [callback.size() + 1];
	
	strcpy(cHost, host.c_str());
	strcpy(cCallback, callback.c_str());
	
	printf("%s\n",cHost);
	printf("%s\n",cCallback);
	
	char *trackerResponse = http_get(cHost, port, cCallback);
	
	return trackerResponse;
}

/*
 * Member functions implementation
 */
string TransportHTTP::doStuff(const string &code)
{
	return "";
}

void TransportHTTP::test()
{
	
}

