/*
 *  http_operations.h
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#ifndef HTTP_OPERATIONS_H
#define HTTP_OPERATIONS_H

#define SA      struct sockaddr
#define MAXLINE 4096
#define MAXSUB  200
#define LISTENQ 1024

#include "HTTP_Client.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>

/*
 *	This function should process an HTTP POST, ex:
 *
 * char *hname		= "www.mydomain.com";
 * char *page		= "/somedir/script.php";
 * char *poststr	= "mode=login&user=test&password=test\r\n";
 *
 */

char* process_http(int sockfd, char *host, char *page, char *poststr);

/*
 *	This function should send a POST to the specified server.
 *
 */

char* sendPost(char *hostName, int port, char *page, char *postString);

/*
 *	This function should return our localIP address.
 */

char* getLocalIp();

/*
 *	This function should extract the POST message from the whole message char*.
 */

int getPostMessage(char *message);

#endif

