/*
 *  HTTP_Client.h
 *  P2P_Engine
 *
 *  Created by LogNet team 2010 - INRIA
 *  Mediteranee - Sophia Antipolis - France
 *
 */

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

/*
 *	This function should perform a GET request and return a char*
 *	with the HTML asociated to the page requested.
 *  ex: http_get("127.0.0.1",80,"/index.html");
 */
char* http_get(char *host, int port, char *page);

#endif
