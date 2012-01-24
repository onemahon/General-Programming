/*******************************************************************

HOST dispatcher
 
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
				perror("startPCB");
				exit(1);
			case 0:
				p->pid = getpid();
				p->status = PCB_RUNNING;
				printPcbHdr(stdout);
				printPcb(p, stdout);
				fflush(stdout);
				execvp(p->args[0], p->args);
				perror(p->args[0]);
				exit(2);
		} // parent     
		

    } else { // already started & suspended so continue
		kill(p->pid, SIGCONT);
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
	 
	 if(kill(p->pid, SIGSTOP)){
		 fprintf(stderr, "terminate of %d failed", (int) p->pid);
		 return NULL;
	 }
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
    
	//terminate process with sigint
	if(kill(p->pid, SIGINT)){
		fprintf(stderr, "terminate of %d failed", (int) p->pid);
		return NULL;
	}
	waitpid(p->pid, &status, WUNTRACED); //what is the purpose of this waiting, and WUNTRACED?
	p->status = PCB_TERMINATED;
	
	//update its status
	
	//remove its PCB from the queue -- DONE IN HOSTD
	
	//free all memory allocated to it
	//free(p);
	//might need to be more thorough here, but this is OK for now
	
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
	newprocessPtr = (PcbPtr)malloc(sizeof(Pcb));
	if(!newprocessPtr){
		perror("Error in creating new process block");
	}
	newprocessPtr->pid = 0;
	newprocessPtr->args[0] = DEFAULT_PROCESS;
	newprocessPtr->args[1] = NULL;
	newprocessPtr->arrivaltime = 0;
	newprocessPtr->priority = 0;
	newprocessPtr->remainingcputime = 0;
	newprocessPtr->mbytes = 0;
	newprocessPtr->status = PCB_UNINITIALIZED;
	newprocessPtr->next = NULL;
	newprocessPtr->prev = NULL;
	return newprocessPtr;
}   

/*******************************************************
 * PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
 *    - queue process (or join queues) at end of queue
 * 
 * returns head of queue
 ******************************************************/
 
PcbPtr enqPcb(PcbPtr q, PcbPtr p)
{
	//adds a PCB into the circular queue
	
    PcbPtr ptr = q;
	if(q == NULL){
		//no pcbs yet
		p->next = p;
		p->prev = p;
		q = p;
	}else{
		while(ptr->next != q){
			ptr = ptr->next;
		}
		ptr->next = p;
		p->next = q;
		p->prev = ptr;
	}
	
	return q;
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
 
PcbPtr deqPcb(PcbPtr *hPtr)
{
    PcbPtr p = *hPtr;
	if(p != NULL){
		if(p->status == PCB_TERMINATED){
			*hPtr = NULL;
			return NULL;
		}else {
			(p->prev)->next = p->next;
			*hPtr = p->next;
			return p;
		}
	}else{
		return NULL;
	}
}

