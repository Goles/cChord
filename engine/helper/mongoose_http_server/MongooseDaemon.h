#ifdef IPHONE //if we are building for OSX/IPHONE.

#ifndef MONGOOSE_DAEMON_H
#define MONGOOSE_DAEMON_H

#import "mongoose.h"

#define WWW_SERVER_PORT 8082

@interface MongooseDaemon : NSObject {
  struct mg_context *ctx;
}

@property (readwrite)          struct mg_context *ctx;

- (void)startMongooseDaemon:(NSString *)ports;
- (NSString *) localIPAddress;
- (void) initCallbacks;
- (void)stopMongooseDaemon;

@end

#endif

#endif