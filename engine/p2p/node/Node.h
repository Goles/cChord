/*
 *  Node.h
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Node
{

public:
	Node(const string &inIp, unsigned int inId, unsigned int inPort);
	Node(const string &ip, unsigned int port);
	Node(const string &data);

	bool	equals(Node *node);
	string	toString();
	vector<string> tokenize(const string& str, const string& delims);

	//Getters & Setters
	unsigned int 	getId() { return nid; }
	void			setId(unsigned  int inId) { nid = inId; }
	string			getIdString(){ return nidS; }
	void			setIdString(const string &s) { nidS = string(s); }
	string			getIp(){ return ip; }
	void			setIp(const std::string inIp) { ip = inIp; }
	unsigned int	getNid() { return nid; }
	void			setNid(unsigned int n) { nid = n; }
	int				getPort() { return port; }
	void			setPort(int inPort){ port = inPort; }

private:
	string			ip;
	unsigned int	nid; //node id (nid), "id" is reserved keyword in Cocoa.
	string			nidS;
	int				port;

};

#endif
