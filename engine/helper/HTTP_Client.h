/*
 *  HTTP_Client.h
 *  P2P_Engine
 *
 *  Created by Nicolas Goles on 2/25/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

/*
 *	This function should perform a GET command and return a char*
 *	with the HTML asociated to the page requested.
 *  host : 127.0.0.1  page /index.html
 */
char* http_get(char *host, int port, char *page);

#endif