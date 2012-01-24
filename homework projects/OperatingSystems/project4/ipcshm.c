/*
 * proj : IPC sample code
 * file : ipcshm.c
 * rev  : 1
 * desc : System V IPC shared memory operations.
 * rcs  : $Id$
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ipcshm.h" 
#include "key.h"    
#include "error.h"  


static char *ptr2Shm = NULL; 
static int  shmId = 0;


int createShm (const int size) {
 /* 
  * input   : Size of the shared memory block.
  * returns : NOK if any errors, OK if none.
  * output  : - 
  * desc    : creates and attaches a shared memory block
  */

  if ((shmId = shmget (getKey(), size, IPC_CREAT | PERMISSIONS)) == 
      SYSTEMERROR) {

//    systemError (__FILE__,__LINE__,"createShm","shmget");
    return (NOK);

  }

  if ((ptr2Shm = shmat(shmId, NULL, 0)) == SYSTEMPTRERROR) {
    
//    systemError (__FILE__,__LINE__,"createShm","shmat");
    return (NOK);
    
  }

  return OK;

}



int destroyShm (void){
 /* 
  * input   : -
  * returns : NOK if any errors, OK if none.
  * output  : - 
  * desc    : destroys a shared memory block.
  */

  struct shmid_ds scratch;

  if (ptr2Shm == NULL)
    return OK;

  if ((shmdt((void*)(ptr2Shm))) == SYSTEMERROR) {

//    systemError (__FILE__,__LINE__,"destoryShm","shmdt");
    return (NOK);

  }

  ptr2Shm = NULL;

  if (shmId == 0)
    return OK;

  if ((shmctl(shmId, IPC_RMID, &scratch)) == SYSTEMERROR) {
    
//    systemError (__FILE__,__LINE__,"destoryShm","shmctl");
    return (NOK);
    
  }

  shmId = 0;
  
  return OK;

}



int detachShm (void){
 /* 
  * input   : -
  * returns : NOK if any errors, OK if none.
  * output  : - 
  * desc    : destroys a shared memory block.
  */


  if (ptr2Shm == NULL)
    return OK;

  if ((shmdt((void*)(ptr2Shm))) == SYSTEMERROR ){

//    systemError (__FILE__,__LINE__,"detachShm","shmdt");
    return (NOK);

  }

  ptr2Shm = NULL;

  return OK;

}



//void writeShm (const int offset, const int size, const char *data){
void writeShm (const int offset, const int size, void *data){
 /* 
  * input   : the offset into shared memory, the size of the data, and a ptr to
  *           the data itself.
  * returns : -
  * output  : - 
  * desc    : writes data into shared memory.
  */

	//printf("adress: %d, size: %d\n",data,size);
	//printf("adress: %d, size: %d\n",ptr2Shm,size);
  memcpy (&ptr2Shm[offset*size],data, size);
//printf("here");
  return;

}



//void readShm (const int offset, const int size, char *data){
void readShm (const int offset, const int size, void *data){
 /* 
  * input   : the offset into shared memory, the size of the data, and a ptr 2
  *           a place where to store the data.
  * returns : -
  * output  : - 
  * desc    : reads data from shared memory.
  */

  memcpy (data,&ptr2Shm[offset*size],size);

  return;

}


void writeShmI (const int offset, const int size, const int *data){
 /*
  * input   : the offset into shared memory, the size of the data, and a ptr to
  *           the data itself.
  * returns : -
  * output  : -
  * desc    : writes data into shared memory.
  */

  memcpy (&ptr2Shm[offset*(sizeof(int))],data, size*(sizeof(int)));

  return;

}



void readShmI (const int offset, const int size, int *data){
 /*
  * input   : the offset into shared memory, the size of the data, and a ptr 2
  *           a place where to store the data.
  * returns : -
  * output  : -
  * desc    : reads data from shared memory.
  */

  memcpy (data,&ptr2Shm[offset*(sizeof(int))],size*(sizeof(int)));

  return;

}

char *getShmPtr (void) {
 /* 
  * input   : -
  * returns : ptr 2 shared memory, can be NULL, if not initialized correctly.
  * output  : - 
  * desc    : returns the value of the internal global variable ptr2Shm.
  */

  return ptr2Shm;

}





