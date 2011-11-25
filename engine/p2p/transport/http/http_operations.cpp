/*
 *  http_operations.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#include "http_operations.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

char* process_http(int sockfd, char *host, char *page, char *poststr) {
	//This is to avoid application crash when we perform write() or read.
	signal(SIGPIPE, SIG_IGN);
	ssize_t n;
	int sendlineSize = strlen(page) + MAXLINE + 1;
	char sendline[sendlineSize];
	char *recvline = (char *) malloc(sizeof(char) * (sendlineSize));

	sprintf(sendline, "POST %s HTTP/1.0\r\n"
		"Host: %s\r\n"
		"Content-type: application/x-www-form-urlencoded\r\n"
		"Content-length: %d\r\n\r\n"
		"%s", page, host, (int) strlen(poststr), poststr);

	if (write(sockfd, sendline, sizeof(sendline)) == -1) {
//		printf("Failed to write to socket! \n");
//		printf("Check if socket is writeable.\n");
//		printf("Errno %d: %s\n", errno, strerror(errno));
		return NULL;
	}

	while ((n = read(sockfd, recvline, sendlineSize)) != 0) {
		recvline[n] = '\0';
	}

	return recvline; //this must be freed by the receiver!! ( allocated here! )
}

char* sendPost(char *hostName, int port, char *page, char *postString) {
	int sockfd;
	struct sockaddr_in servaddr;

	char **pptr;
	char *response = NULL;
	char str[50];
	struct hostent *hptr;

	//TODO: Still have to make all the checks if connect fails etc.
	if ((hptr = gethostbyname(hostName)) == NULL) {
		fprintf(stderr, " gethostbyname error for host: %s: %s", hostName,
				hstrerror(h_errno));
		return NULL;
	}

	//printf("hostname: %s\n", hptr->h_name);

	if (hptr->h_addrtype == AF_INET && (pptr = hptr->h_addr_list) != NULL) {
		//printf("address: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
		inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));
	} else {
		fprintf(stderr, "Error call inet_ntop \n");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET, str, &servaddr.sin_addr);

	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	if ((response = process_http(sockfd, hostName, page, postString)) == NULL) {
		close(sockfd);
		free(response);
		return NULL; //false.
	}

	close(sockfd);

	//The response contains the server HTML answer, the receiver MUST free it or will leak.
	return response;
}

char* getLocalIp() {
	//First we get the host-name
	char hostName[255];
	gethostname(hostName, 255);

	//We need to fill a hostent struct
	struct hostent *host_entry;
	host_entry = gethostbyname(hostName);

	//From the hostent struct we obtain our local IP.
	char * hostIP = NULL;

	if (host_entry != NULL)
		hostIP = inet_ntoa(*(struct in_addr *) *host_entry->h_addr_list);

	return hostIP;
}

/*
 * This function assumes the string passed is in the format AAAAAA,POSTMESAGE.
 * so just extracts the second part of the comma separated string and returns the
 * (in this case) ChordTransportCode number according to the message.
 */

int getPostMessage(char *message) {
	int a = -1;
	char *postMessage = NULL;

	postMessage = strtok(message, ",");

	while (postMessage != NULL) {
		postMessage = strtok(NULL, ",");
		if (postMessage != NULL)
			a = atoi(postMessage); //convert the number in "string" format to an int.
	}

	return a;
}
