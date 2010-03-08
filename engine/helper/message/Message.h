/*
 *  Message.h
 *  P2P_Engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

using std::string;
using std::stringstream;

class Message {



public:
	/* CONSTRUCTOR */
	Message(string overlayID, int code, string message);

	/* GETTER */
	int 	getCode() const { return code; }
	string 	getMessage() const { return message; }
	string 	getOverlayID() const { return overlayID; }

	/* SETTER */
	void setCode(int code) { this->code = code; }
	void setMessage(string message) { this->message = message; }
	void setOverlayID(string overlayID) { this->overlayID = overlayID; }

	/* Serialization */
	string serialize(void);

private:
	string 	overlayID; // Overlay Identifier
	int		code; // see ChordTransportCode.h
	string 	message; // message content (can be empty)
};

#endif
