/*
 *  Message.cpp
 *  P2P_Engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

#include "Message.h"
#include <sstream>
#include <iostream>

Message::Message(string overlayID, int code, string message){
	this->overlayID = overlayID;
	this->code		= code;
	this->message 	= message;
}

string Message::serialize(void){
	stringstream ss (stringstream::in | stringstream::out);
	ss << overlayID << ","<< code << "," << message;
	return ss.str();
}
