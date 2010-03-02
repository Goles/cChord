//
//  MongooseDaemon.m
//
//  Created by Rama McIntosh on 3/4/09.
//  Copyright Rama McIntosh 2009. All rights reserved.
//

//
// Copyright (c) 2009, Rama McIntosh All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
// * Neither the name of Rama McIntosh nor the names of its
//   contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

// MongooseDaemon is a small wrapper to make ingetrating mongoose
// with iPhone apps super easy

#ifdef IPHONE

#import "MongooseDaemon.h"
#import	"callbacks.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DOCUMENTS_FOLDER NSHomeDirectory()


@implementation MongooseDaemon

@synthesize ctx;

// Return the localized IP address
- (NSString *) localIPAddress
{
	char baseHostName[255];
	gethostname(baseHostName, 255);
	
	// Adjust for iPhone -- add .local to the host name
	char hn[255];
	sprintf(hn, "%s.local", baseHostName);
	
	struct hostent *host = gethostbyname(hn);
    if (host == NULL)
	{
        herror("resolv");
		return NULL;
	}
    else {
        struct in_addr **list = (struct in_addr **)host->h_addr_list;
		return [NSString stringWithCString:inet_ntoa(*list[0])];
    }
	
	return NULL;
}

- (void)startHTTP:(NSString *)ports
{
  self.ctx = mg_start();     // Start Mongoose serving thread
  mg_set_option(ctx, "root", [DOCUMENTS_FOLDER UTF8String]);  // Set document root
  mg_set_option(ctx, "ports", [ports UTF8String]);    // Listen on port XXXX

  // Now Mongoose is up, running and configured.
  // Serve until somebody terminates us
  NSLog(@"** HTTP SERVER INIT **\n");	
  NSLog(@"Mongoose Server is running on http://%@:8080", [self localIPAddress]);
}

- (void) initCallbacks
{
	/*Test callbacks*/
	mg_set_uri_callback(ctx, "/requesthandler", &call_request_handler, NULL);
	mg_set_uri_callback(ctx, "/ping", &call_ping, NULL);
	mg_set_uri_callback(ctx, "/pong", &call_pong, NULL);
	
	/*Set tracker transport code Callbacks*/
	mg_set_uri_callback(ctx, "/tracker/addnode", &call_tracker_addnode, NULL);
	mg_set_uri_callback(ctx, "/tracker/getconnection", &call_tracker_getconnection, NULL);
	mg_set_uri_callback(ctx, "/tracker/removenode", &call_tracker_removenode, NULL);	
	mg_set_uri_callback(ctx, "/tracker/join", &call_tracker_join, NULL);	
	
	/*Set Chord transport code Callbacks*/
	mg_set_uri_callback(ctx, "/getpred", &call_chord_getpred, NULL);
	mg_set_uri_callback(ctx, "/findsucc", &call_chord_findsucc, NULL);
	mg_set_uri_callback(ctx, "/notif", &call_chord_notif, NULL);
	mg_set_uri_callback(ctx, "/join", &call_chord_join, NULL);
	mg_set_uri_callback(ctx, "/put", &call_chord_put, NULL);
	mg_set_uri_callback(ctx, "/get", &call_chord_get, NULL);
	mg_set_uri_callback(ctx, "/setsucc", &call_chord_setsucc, NULL);
	mg_set_uri_callback(ctx, "/setpred", &call_chord_setpred, NULL);
}

- (void)startMongooseDaemon:(NSString *)ports;
{
  [self startHTTP:ports];
  [self initCallbacks];
}

- (void)stopMongooseDaemon
{
  mg_stop(ctx);
}

@end

#endif
