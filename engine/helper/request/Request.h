/*
 *  Request.h
 *  P2P_Engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010

 *
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>

using std::endl;
using std::cout;
using std::string;
using std::stringstream;
using std::map;
using std::ostringstream;

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
	Request(string overlayID, int code);

	/* GETTER */
	int getCode() const {
		return code;
	}
	string getOverlayID() const {
		return overlayID;
	}

	/* SETTER */
	void setCode(int code) {
		this->code = code;
	}
	void setOverlayID(string overlayID) {
		this->overlayID = overlayID;
	}

	/*Methods*/
	void addArg(string key, string value);
	string getArg(string key);
	string serialize();
	unsigned int getCheckSum(string str);

private:
	typedef std::pair<string, string> arg;
	typedef std::map<string, string> argMap;

	string overlayID; // Overlay Identifier
	int code; // see ChordTransportCode.h

	argMap arguments;
};

#endif
