/*
 *  Node.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/14/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#ifndef NODE_H
#define NODE_H

#include <string>

using namespace std;

class Node
{
	
public:
	Node(const string &inIp, unsigned int inId, int inPort);
	Node(const string &ip, int port);
	Node(const string &data);
	
	bool	equals(Node *node);
	string	toString();

	//Getters & Setters
	int		getId() { return nid; }
	void	setId(int inId) { nid = inId; }
	string	getIp(){ return ip; }
	void	setIp(const std::string inIp) { ip = inIp; }
	unsigned int getNid() { return nid; }
	void	setNid(unsigned int n) { nid = n; }
	int		getPort() { return port; }
	void	setPort(int inPort){ port = inPort; }

private:
	string			ip;
	unsigned int	nid; //node id (nid), "id" is reserved keyword in Cocoa.
	int				port;
};

#endif