/*
 *  Request.h
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

/*
 * Request format:
 * <OverlayID>,<RequestCode>[,<args>]*
 *
 * RequestCode depend of the overlay.
 * Example with chord: see ChordTransportCode.h
 */
class Request {

public:
	/* CONSTRUCTOR */
	Request(string overlayID, int code, string request);

	/* GETTER */
	int 	getCode() const { return code; }
	string 	getRequest() const { return request; }
	string 	getOverlayID() const { return overlayID; }

	/* SETTER */
	void setCode(int code) { this->code = code; }
	void setRequest(string request) { this->request = request; }
	void setOverlayID(string overlayID) { this->overlayID = overlayID; }

	/* Serialization */
	string serialize(void);

private:
	string 	overlayID; // Overlay Identifier
	int		code; // see ChordTransportCode.h
	string 	request; // request content (can be empty)
};

#endif
