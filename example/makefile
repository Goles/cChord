CC = /usr/bin/g++
CC_OPTIONS = -I.


#if we passed ARCH=osx
ifeq ($(ARCH),osx)
	@echo Building for OSX ( ARCH=osx )
	CC_OPTIONS = -DOSX=1
endif

#if we passed ARCH=linux
ifeq ($(ARCH),linux)
	@echo Building for LINUX ( ARCH=linux )
	LNK_OPTIONS = -lpthread -lrt -ldl
endif

all: example.out
	
example.out: example.cpp
	$(CC) $(CCFLAGS) $(LNK_OPTIONS) example.cpp libmymed.a -o example.out

clean:
	rm *.out