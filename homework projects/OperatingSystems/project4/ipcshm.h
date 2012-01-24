/*
 * proj : IPC sample code
 * file : ipcshm.c
 * rev  : 1
 * desc : System V IPC shared memory operations definitions.
 * rcs  : $Id$
 *
 */

#ifndef IPCSHM_H
#define IPCSHM_H

#ifndef PERMISSIONS
#define PERMISSIONS 0666
#endif

int createShm (const int size) ;
 /* 
  * input   : Size of the shared memory block.
  * returns : NOK if any errors, OK if none.
  * output  : - 
  * desc    : creates and attaches a shared memory block
  */

int destroyShm (void);
 /* 
  * input   : -
  * returns : NOK if any errors, OK if none.
  * output  : - 
  * desc    : destroys a shared memory block.
  */

int detachShm (void);
 /* 
  * input   : -
  * returns : NOK if any errors, OK if none.
  * output  : - 
  * desc    : destroys a shared memory block.
  */

//void writeShm (const int offset, const int size, const char *data);
void writeShm (const int offset, const int size, void *data);
 /* 
  * input   : the offset into shared memory, the size of the data, and a ptr to
  *           the data itself.
  * returns : -
  * output  : - 
  * desc    : writes data into shared memory.
  */

//void readShm (const int offset, const int size, char *data);
void readShm (const int offset, const int size, void *data);

 /* 
  * input   : the offset into shared memory, the size of the data, and a ptr 2
  *           a place where to store the data.
  * returns : -
  * output  : - 
  * desc    : reads data from shared memory.
  */

void writeShmI (const int offset, const int size, const int *data);
 /*
  * input   : the offset into shared memory, the size of the data, and a ptr to
  *           the data itself. Note that the data is an integer.
  * returns : -
  * output  : -
  * desc    : writes data into shared memory.
  */

void readShmI (const int offset, const int size, int *data);
 /*
  * input   : the offset into shared memory, the size of the data, and a ptr 2
  *           a place where to store the data. Note that the data is an integer.
  * returns : -
  * output  : -
  * desc    : reads data from shared memory.
  */

void writeShmS (const int offset, const int size, const char *data);
 /* 
  * input   : the offset into shared memory, the size of the data, and a ptr to
  *           the data itself. Note that the data can be a struct.
  * returns : -
  * output  : - 
  * desc    : writes data into shared memory.
  */

void readShmS (const int offset, const int size, char *data);
 /* 
  * input   : the offset into shared memory, the size of the data, and a ptr 2
  *           a place where to store the data. Note that the data can be a struct.
  * returns : -
  * output  : - 
  * desc    : reads data from shared memory.
  */

char *getShmPtr (void) ;
 /* 
  * input   : -
  * returns : ptr 2 shared memory, can be NULL, if not initialized correctly.
  * output  : - 
  * desc    : returns the value of the internal global variable ptr2Shm.
  */

#endif
