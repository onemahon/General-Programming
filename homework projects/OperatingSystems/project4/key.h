
/*
 * proj : IPC sample code
 * file : key.c
 * rev  : 1
 * desc : Unique key generation module definitions
 * rcs  : $Id$
 *
 */

#ifndef KEY_H
#define KEY_H

#include <sys/types.h>

/* 
 * Some standard defs, makes code easier to read.
 */

#define NULLKEY (key_t) -1




key_t getKey(void);
 /* 
  * input   : -
  * returns : key, in case of an error it returns a NULLKEY
  * output  : -
  * desc    : creates a unique key based on an i-node number 
  *           (in this case a users home-dir is used) and a 
  *           project number.
  */


#endif
