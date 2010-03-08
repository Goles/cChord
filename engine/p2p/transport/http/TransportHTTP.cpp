/*
 *  TransportHTTP.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/26/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#ifdef XCODE
	#include "http_operations.c" //I include .c file for the linker to notice the .o
#else
	#include "http_operations.h"
#endif

#include "TransportHTTP.h"
#include "HTTP_Client.h"
#include "ChordNode.h"
#include "IOverlay.h"
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
	
	//Initialize the webserver
	this->startHTTP();
}

TransportHTTP::~TransportHTTP()
{
	this->stopHTTP();
}

void TransportHTTP::startHTTP()
{
	stringstream portStream (stringstream::in | stringstream::out);
	portStream << port;
	
	ctx = mg_start();     // Start Mongoose serving context thread
	mg_set_option(ctx, "root", ".");  // Set document root
	mg_set_option(ctx, "ports", portStream.str().c_str());    // Listen on port XXXX

	/* Now Mongoose is up, running and configured.
	   Serve until somebody terminates us */
	
	cout << "Mongoose Server is running on http://localhost:" << port << endl;
}

void TransportHTTP::stopHTTP()
{
	mg_stop(ctx);
	cout << "Mongoose Server is now stopped" << endl;
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
string TransportHTTP::sendRequest(const string &message, Node *destination)
{	
	int a = atoi(message.c_str());
	
	string *callback;
	
	
	//We need to forge the corresponding callback string for the message passed.
	
	switch (a) {
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
			assert(a);
			break;
	}
	
	return(this->sendRequest(callback, message, destination));
}

/*
 *	This function is here to send message to other callbacks of the webserver (in case there are).
 */
string TransportHTTP::sendRequest(string *callback, const string &message, Node *destination)
{		
	cout << "\nMessage: " << message << endl;
	cout << "Callback: " << *callback << endl;
	cout << "Destination: " << destination->toString() << endl;
	
	char *response = NULL;
	
	/*
	 *	We should send our POST to the destination, their callback function,
	 *	must manage it accordingly, and the callback that we choose must be passed
	 *	via the callback argument of this function.
	 */
	
	response = sendPost((char *)(destination->getIp()).c_str(), 
						destination->getPort(), 
						(char *)callback->c_str(), 
						(char *)message.c_str());
	
	//if we have null response from sendPost()
	if(response == NULL)
		return string("");
	
	//else
	string cpp_response = string(response);
	//delete callback;
	free(response); // we must free the initial char* response, to avoid leaks.
	return cpp_response;
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

