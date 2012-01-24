/*******************************************************************
 pcb - process control block functions for HOST dispatcher

   PcbPtr startPcb(PcbPtr process) - start (or restart) a process
    returns:
      PcbPtr of process
      NULL if start (restart) failed

   PcbPtr suspendPcb(PcbPtr process) - suspend a process
    returns:
      PcbPtr of process
      NULL if suspend failed

   PcbPtr terminatePcb(PcbPtr process) - terminate a process
    returns:
      PcbPtr of process
      NULL if terminate failed

   PcbPtr printPcb(PcbPtr process, FILE * iostream)
    - print process attributes on iostream
    returns:
      PcbPtr of process

   void printPcbHdr(FILE *) - print header for printPcb
    returns:
      void
      
   PcbPtr createnullPcb(void) - create inactive Pcb.
    returns:
      PcbPtr of newly initialised Pcb
      NULL if malloc failed

   PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
      - queue process (or join queues) at end of queue
      - enqueues at "tail" of queue. 
    returns head of queue

   PcbPtr deqPcb (PcbPtr * headofQ);
      - dequeue process - take Pcb from "head" of queue.
    returns:
      PcbPtr if dequeued,
      NULL if queue was empty
      & sets new head of Q pointer in adrs at 1st arg
 *******************************************************************/

#include "pcb.h"

/*******************************************************
 * PcbPtr startPcb(PcbPtr process) - start (or restart)
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if start (restart) failed
 ******************************************************/
PcbPtr startPcb (PcbPtr p) 
{ 
    if (p->pid == 0) {                 // not yet started
        switch (p->pid = fork ()) {    //  so start it
            case -1: 
                perror ("startPcb");
                exit(1); 
            case 0:                             // child 
                p->pid = getpid();
                p->status = PCB_RUNNING;
                printPcbHdr(stdout);            // printout in child to
                printPcb(p, stdout);            //  sync with o/p
                fflush(stdout);
                execvp (p->args[0], p->args); 
                perror (p->args[0]);
                exit (2);
        }                                       // parent         

    } else { // already started & suspended so continue
        kill (p->pid, SIGCONT);
    }    
    p->status = PCB_RUNNING;
    return p; 
} 

/*******************************************************
 * PcbPtr suspendPcb(PcbPtr process) - suspend
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if suspend failed
 ******************************************************/
 PcbPtr suspendPcb(PcbPtr p)
 {
     int status;
     
     kill(p->pid, SIGTSTP);
     waitpid(p->pid, &status, WUNTRACED);
     p->status = PCB_SUSPENDED;
     return p;
 }
 
/*******************************************************
 * PcbPtr terminatePcb(PcbPtr process) - terminate
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if terminate failed
 ******************************************************/
PcbPtr terminatePcb(PcbPtr p)
{
    int status;
    
    kill(p->pid, SIGINT);
    waitpid(p->pid, &status, WUNTRACED);
    p->status = PCB_TERMINATED;
    return p;
}  

/*******************************************************
 * PcbPtr printPcb(PcbPtr process, FILE * iostream)
 *  - print process attributes on iostream
 *  returns:
 *    PcbPtr of process
 ******************************************************/
 
PcbPtr printPcb(PcbPtr p, FILE * iostream)
{
    fprintf(iostream, "%7d%7d%7d%7d  ",
        (int) p->pid, p->arrivaltime, p->priority,
            p->remainingcputime);
//        p->memoryblock->offset, p->mbytes,
//        p->req.printers, p->req.scanners,p->req.modems,p->req.cds);
    switch (p->status) {
        case PCB_UNINITIALIZED:
            fprintf(iostream, "UNINITIALIZED");
            break;
        case PCB_INITIALIZED:
            fprintf(iostream, "INITIALIZED");
            break;
        case PCB_READY:
            fprintf(iostream, "READY");
            break;
        case PCB_RUNNING:
            fprintf(iostream, "RUNNING");
            break;
        case PCB_SUSPENDED:
            fprintf(iostream, "SUSPENDED");
            break;
        case PCB_TERMINATED:
            fprintf(iostream, "PCB_TERMINATED");
            break;
        default:
            fprintf(iostream, "UNKNOWN");
    }
    fprintf(iostream,"\n");
    
    return p;     
}
   
/*******************************************************
 * void printPcbHdr(FILE *) - print header for printPcb
 *  returns:
 *    void
 ******************************************************/  
 
void printPcbHdr(FILE * iostream) 
{  
//    fprintf(iostream,"    pid arrive  prior    cpu offset Mbytes     prn    scn   modem   cd  status\n");
    fprintf(iostream,"    pid arrive  prior    cpu  status\n");

}
       
/*******************************************************
 * PcbPtr createnullPcb() - create inactive Pcb.
 *
 * returns:
 *    PcbPtr of newly initialised Pcb
 *    NULL if malloc failed
 ******************************************************/
 
PcbPtr createnullPcb()
{
    PcbPtr newprocessPtr;
      
    if ((newprocessPtr = (PcbPtr) malloc (sizeof(Pcb)))) {
        newprocessPtr->pid = 0;
        newprocessPtr->args[0] = DEFAULT_PROCESS;
        newprocessPtr->args[1] = NULL;
        newprocessPtr->arrivaltime = 0;
        newprocessPtr->priority = HIGH_PRIORITY;
        newprocessPtr->remainingcputime = 0;
        newprocessPtr->mbytes = 0;
//        newprocessPtr->memoryblock = NULL;
        newprocessPtr->req.printers = 0;
        newprocessPtr->req.scanners = 0;
        newprocessPtr->req.modems = 0;
        newprocessPtr->req.cds = 0;
        newprocessPtr->status = PCB_UNINITIALIZED;
        newprocessPtr->next = NULL;
        return newprocessPtr;
    }
    perror("allocating memory for new process");
    return NULL;
}   

/*******************************************************
 * PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
 *    - queue process (or join queues) at end of queue
 * 
 * returns head of queue
 ******************************************************/
 
PcbPtr enqPcb(PcbPtr q, PcbPtr p)
{
    PcbPtr h = q;
    
    p->next = NULL; 
    if (q) {
        while (q->next) q = q->next;
        q->next = p;
        return h;
    }
    return p;
}
/*******************************************************
 * PcbPtr deqPcb (PcbPtr * headofQ);
 *    - dequeue process - take Pcb from head of queue.
 *
 * returns:
 *    PcbPtr if dequeued,
 *    NULL if queue was empty
 *    & sets new head of Q pointer in adrs at 1st arg
 *******************************************************/
 
PcbPtr deqPcb(PcbPtr * hPtr)
{
    PcbPtr p;
     
    if (hPtr && (p = * hPtr)) {
        * hPtr = p->next;
        return p;
    }
    return NULL;
}

