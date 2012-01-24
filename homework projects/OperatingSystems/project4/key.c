
/*
 * proj : IPC sample code
 * file : key.c
 * rev  : 1
 * desc : Unique key generation for IPC stuff (msgq's sem's and 
 *        shm)
 * rcs  : $Id$
 *
 */


#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>

#include "error.h"
#include "key.h"

static key_t ipcKey = NULLKEY;



key_t getKey(void) {
 /* 
  * input   : -
  * returns : key, in case of an error it returns a NULLKEY
  * output  : -
  * desc    : creates a unique key based on an i-node number 
  *           (in this case a users home-dir is used) and a 
  *           project number.
  *           The line where the key is created is somewhat
  *           funky, based on the user id the passwd file is
  *           interrogated for the homedirectory. Besides that
  *           the user id is used as a project id for the ftok 
  *           call.
  */

  if (ipcKey == NULLKEY) {

    if ((ipcKey = ftok(getpwuid(getuid())->pw_dir,getuid()))
	== SYSTEMERROR){
      
//      systemError (__FILE__,__LINE__,"getKey","ftok");
      ipcKey = NULLKEY;
      
    } 

  }

  return ipcKey;

}
