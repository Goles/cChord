###################################################
#
# Makefile for MYMEDLIB MAC OS X.
# Creator [Xcode -> Makefile Ver: Feb 14 2007 09:18:41]
# Created: [Mon Mar  1 11:40:48 2010]
#
###################################################

#
# Macros
# 

SHELL := /bin/bash

CC = /usr/bin/g++
LNK_OPTIONS = 
CCFLAGS =

#@echo ---=/\=\/= Building myMedLib =\/=/\=--

#if we passed ARCH=osx
ifeq ($(ARCH),osx)
	#@echo Building for OSX ( ARCH=osx )
	CC_OPTIONS = -DOSX=1
endif

#if we passed ARCH=linux
ifeq ($(ARCH),linux)
	#@echo Building for LINUX ( ARCH=linux )
	LNK_OPTIONS = -lpthread -lrt -ldl
endif

HEADER = $(shell for file in `find . -name *.h`;do echo $$file; done)

#
# INCLUDE directories for MYMEDLIB
#

INCLUDE = -I.\
		-Iengine/helper\
		-Iengine/helper/crypto/\
		-Iengine/helper/mongoose_http_server/callbacks\
		-Iengine/helper/mongoose_http_server\
		-Iengine/helper/thread\
		-Iengine/helper/request\
		-Iengine/p2p/node\
		-Iengine/p2p/protocols/chord\
		-Iengine/p2p/protocols\
		-Iengine/p2p/transport/http\
		-Iengine/p2p/transport

#
# Build MYMEDLIB
#
all: MYMEDLIB libmymed.a

MYMEDLIB : \
		./HTTP_Client.o\
		./sha1.o\
		./callbacks.o\
		./chord_callbacks.o\
		./mongoose.o\
		./Thread.o\
		./Node.o\
		./AbstractChord.o\
		./ChordNode.o\
		./ProtocolSingleton.o\
		./http_operations.o\
		./TransportHTTP.o\
		./Request.o
	

# actualy archive the static library with ar.
libmymed.a:
	@echo Archiving myMedLib...
	rm -f libmymed.a
	ar -cvq libmymed.a *.o

clean : 
		rm -f \
		./HTTP_Client.o\
		./sha1.o\
		./callbacks.o\
		./chord_callbacks.o\
		./mongoose.o\
		./Thread.o\
		./Node.o\
		./AbstractChord.o\
		./ChordNode.o\
		./ProtocolSingleton.o\
		./http_operations.o\
		./TransportHTTP.o\
		./Request.o
		
veryclean : clean
		rm -f \
		./libmymed.a\

install : MYMEDLIB
	@echo --= HEADERS =--
	@echo $(HEADER)
	@echo --= CLEANING OLD HEADERS =--
	mkdir -p /usr/include/myMed
	@echo --= INSTALLING MYMEDLIB HEADERS =--
	cp $(HEADER) /usr/include/myMed

#
# Build the parts of MYMEDLIB
#

# Item # 2 -- HTTP_Client --
./HTTP_Client.o : engine/helper/HTTP_Client.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/HTTP_Client.cpp -c $(INCLUDE) -o ./HTTP_Client.o


## Item # 3 -- crypt_argchk --
#./crypt_argchk.o : engine/helper/crypto/tomcrypt/algorithms/crypt_argchk.c
#	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/crypto/tomcrypt/algorithms/crypt_argchk.c -c $(INCLUDE) -o ./crypt_argchk.o
#
## Item # 4 -- sha1 --
#./sha1.o : engine/helper/crypto/tomcrypt/algorithms/sha1.c
#	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/crypto/tomcrypt/algorithms/sha1.c -c $(INCLUDE) -o ./sha1.o
#
## Item # 5 -- md5 --
#./md5.o : engine/helper/crypto/tomcrypt/algorithms/md5.c
#	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/crypto/tomcrypt/algorithms/md5.c -c $(INCLUDE) -o ./md5.o

# Item # 5 -- sha1 --
./sha1.o: engine/helper/crypto/sha1.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/crypto/sha1.cpp -c $(INCLUDE) -o ./sha1.o
	
# Item # 6 -- callbacks --
./callbacks.o : engine/helper/mongoose_http_server/callbacks/callbacks.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/mongoose_http_server/callbacks/callbacks.cpp -c $(INCLUDE) -o ./callbacks.o

# Item # 8 -- chord_callbacks --
./chord_callbacks.o : engine/helper/mongoose_http_server/callbacks/chord_callbacks.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/mongoose_http_server/callbacks/chord_callbacks.cpp -c $(INCLUDE) -o ./chord_callbacks.o


# Item # 9 -- mongoose --
./mongoose.o : engine/helper/mongoose_http_server/mongoose.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/mongoose_http_server/mongoose.cpp -c $(INCLUDE) -o ./mongoose.o


# Item # 10 -- Thread --
./Thread.o : engine/helper/thread/Thread.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/thread/Thread.cpp -c $(INCLUDE) -o ./Thread.o


# Item # 11 -- Node --
./Node.o : engine/p2p/node/Node.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/p2p/node/Node.cpp -c $(INCLUDE) -o ./Node.o


# Item # 12 -- AbstractChord --
./AbstractChord.o : engine/p2p/protocols/chord/AbstractChord.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/p2p/protocols/chord/AbstractChord.cpp -c $(INCLUDE) -o ./AbstractChord.o


# Item # 13 -- ChordNode --
./ChordNode.o : engine/p2p/protocols/chord/ChordNode.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/p2p/protocols/chord/ChordNode.cpp -c $(INCLUDE) -o ./ChordNode.o


# Item # 14 -- ProtocolSingleton --
./ProtocolSingleton.o : engine/p2p/protocols/ProtocolSingleton.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/p2p/protocols/ProtocolSingleton.cpp -c $(INCLUDE) -o ./ProtocolSingleton.o


# Item # 15 -- http_operations --
./http_operations.o : engine/p2p/transport/http/http_operations.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/p2p/transport/http/http_operations.cpp -c $(INCLUDE) -o ./http_operations.o


# Item # 16 -- TransportHTTP --
./TransportHTTP.o : engine/p2p/transport/http/TransportHTTP.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/p2p/transport/http/TransportHTTP.cpp -c $(INCLUDE) -o ./TransportHTTP.o
	
# Item # 17 -- TransportHTTP --
./Request.o : engine/helper/request/Request.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/request/Request.cpp -c $(INCLUDE) -o ./Request.o


##### END RUN ####
