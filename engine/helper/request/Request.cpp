/*
 *  Request.cpp
 *  P2P_Engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010

 *
 */

#include "Request.h"
#include "ChordTransportCode.h"

/* Constructor */
Request::Request(string overlayID, int code) {
	this->overlayID = overlayID;
	this->code = code;
}

/* Adds an arument to the Request arguments Map. */
void Request::addArg(string key, string value) {
	arguments.insert(arg(key, value));
}

/* Request an argument to the request arguments Map. */
string Request::getArg(string key) {
	argMap::iterator it = arguments.find(key);

	if (it != arguments.end()) {
		return (it->second);
	}

	return "";
}

unsigned int Request::getCheckSum(string str) {

//	hash_state md;
//	unsigned char out[16];
//
//	//Initialize the MD5 hash function according to Tomcrypt library.
//	md5_init(&md);
//
//	//Apply SHA-1 to the input string.
//	md5_process(&md, (unsigned char *) str.c_str(), str.length());
//
//	//Get the hash output.
//	md5_done(&md, out);
//
//	// transform the md5 string to an integer
//	unsigned int md5toInt = 0;
//	for (int i = 0; i < strlen((const char *) out) + 1; i++)
//		md5toInt = md5toInt * 256 + (out[i] & 0xff);

	return 0;
}

/* Serialize the whole request into a callback string. "/callback?var1=a&var2=b....&var1000=bar" */
string Request::serialize() {
	argMap::iterator it;

	ostringstream ss;
	string *callback;

	switch (this->getCode()) {
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
	case REMOVEKEY:
		callback = new string("/removekey");
		break;
	case SETSUCC:
		callback = new string("/setsucc");
		break;
	case SETPRED:
		callback = new string("/setpred");
		break;
	default:
		cout << " UNHANDLED CHORD TRANSPORT CODE! ... ASSERTING" << endl;
		assert(this->getCode());
		break;
	}

	ss << (*callback) << "?";
	for (it = arguments.begin(); it != arguments.end(); ++it) {
		ss << (*it).first << "=" << (*it).second << "&";
	}
	ss << "checksum=" << getCheckSum(ss.str());
//	cout << ss.str() << endl;
	return ss.str();
}

