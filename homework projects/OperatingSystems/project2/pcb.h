#define PCB_H
/*******************************************************************

  HOST dispatcher

  pcb - process control block functions for HOST dispatcher

  PcbPtr startPcb(PcbPtr process) - start (or restart) a process
  PcbPtr suspendPcb(PcbPtr process) - suspend a process
  PcbPtr terminatePcb(PcbPtr process) - terminate a process
  PcbPtr printPcb(PcbPtr process, FILE * iostream)
  void printPcbHdr(FILE *) - print header for printPcb
  PcbPtr createnullPcb(void) - create inactive Pcb.
  PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
  PcbPtr deqPcb (PcbPtr * headofQ);

  see pcb.c for fuller description of function arguments and returns

*******************************************************************/


//#include "mab.h"
//#include "rsrc.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h> 

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* process management definitions *****************************/

#define MAXARGS 3

#define DEFAULT_PROCESS "./process"

#define N_QUEUES         4  /* number of queues (including RT) */
#define RT_PRIORITY      0
#define HIGH_PRIORITY    1
#define LOW_PRIORITY     (N_QUEUES - 1)

#define PCB_UNINITIALIZED 0
#define PCB_INITIALIZED 1
#define PCB_READY 2
#define PCB_RUNNING 3
#define PCB_SUSPENDED 4
#define PCB_TERMINATED 5

struct rsrc {
    int printers;
    int scanners;
    int modems;
    int cds;
};

typedef struct rsrc Rsrc;
typedef Rsrc * RsrcPtr;

struct pcb {
    pid_t pid;
    char * args[MAXARGS];
    int arrivaltime;
    int priority;
    int remainingcputime;
    int mbytes;
//    MabPtr memoryblock;
    Rsrc req;
    int status;
    struct pcb * next;
    struct pcb * prev;
}; 

typedef struct pcb Pcb;
typedef Pcb * PcbPtr;

/* process management prototypes *****************************/

PcbPtr startPcb(PcbPtr);
PcbPtr suspendPcb(PcbPtr);
PcbPtr terminatePcb(PcbPtr);
PcbPtr printPcb(PcbPtr, FILE *);
void   printPcbHdr(FILE *);
PcbPtr createnullPcb();
PcbPtr enqPcb(PcbPtr, PcbPtr);
PcbPtr deqPcb(PcbPtr*);
