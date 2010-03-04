/*
 *  sha1toint.h
 *  iPhone_p2p_engine
 *
 *  Created by Nicolas Goles on 1/20/10.
 *  Copyright 2010 INRIA Lognet. All rights reserved.
 *
 */

#ifndef SHA1TOINT_H_
#define SHA1TOINT_H_

#include <string.h>

/**
 * MAX_BYTE_NUM 20, assures that for a provided SHA-1 string,
 * the same output unsigned int will be given on 32 or 64 bit
 * platforms. Ex: "<192.168.0.1008080>" -> 2851085774 (unsigned int)
 */

#define MAX_BYTE_NUM 20

/**
 * This function takes an "unsigned char*" and transforms it to an 
 * unsigned int. The Unsigned char* ensures multi platform consistent
 * hashing ( provided by tomcrypt library ).
 *
 * This is the easiest way to do it, as can be read in 
 * http://bytes.com/topic/c/answers/221930-question-converting-unsigned-char-int
 *
 * NG 20 - Jan - 2010
 */

static inline unsigned int sha1ToInt(unsigned char *sha1)
{
	unsigned int num = 0;
	
	int i;
	
	for (i = 0; i < MAX_BYTE_NUM; i++)
		num = num * 256 + (sha1[i] & 0xff);
	
	return num;
}

#endif
