/*
 *  TransportHTTP.h
 *  iPhone_p2p_engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

#ifndef TRANSPORT_HTTP_H
#define TRANSPORT_HTTP_H

#include "ITransport.h"
#include "mongoose.h"
#include <iostream>
#include <string>
#include <sstream>

class ChordNode;

class TransportHTTP: public ITransport {
public:
	//Constructor & Destructor.
	TransportHTTP(int port);
	~TransportHTTP();

	//API methods
	char* sendRequest(Request *request, Node *destination);
	string sendTrackerRequest(const string &host, int port,
			const string &callback);
	string doStuff(const string &code);
	void test();

	//HTTP server methods
	void startHTTP();
	void stopHTTP();
	void initCallbacks();

	//Simple tracker interface.
	string connectToTracker(const string &ip, int port, Node *n);

private:
	struct mg_context *ctx;

protected:
	int myPort;
};

#endif
