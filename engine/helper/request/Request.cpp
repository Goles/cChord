/*
 *  Message.cpp
 *  P2P_Engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

#include "Request.h"
#include <sstream>
#include <iostream>

Request::Request(string overlayID, int code, string request){
	this->overlayID = overlayID;
	this->code		= code;
	this->request 	= request;
}

string Request::serialize(void){
	stringstream ss (stringstream::in | stringstream::out);
	ss << overlayID << ","<< code << "," << request;
	return ss.str();
}
