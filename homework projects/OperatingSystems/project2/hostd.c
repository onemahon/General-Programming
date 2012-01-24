/*******************************************************************
HOST dispatcher 

    hostd

        hostd is fcfs 'dispatcher' that reads in a list of 'jobs' from a file
        and 'dispatches' them in a first-come-first-served manner.

        time resolution is one second (although this can be changed). 

    usage
   
        hostd <dispatch file>
 
        where
            <dispatch file> is list of process parameters as specified 
                for assignment 2.

    functionality

    1. Initialize dispatcher queue;
    2. Fill dispatcher queue from dispatch list file;
    3. Start dispatcher timer;
    4. While there's anything in the queue or there is a currently running process:
        i. If a process is currently running;
            a. Decrement process remainingcputime;
            b. If times up:
                A. Send SIGINT to the process to terminate it;
                B. Free up process structure memory
       ii. If no process currently running &&
            dispatcher queue is not empty &&
            arrivaltime of process at head of queue is <= dispatcher timer:
            a. Dequeue process and start it (fork & exec)
            b. Set it as currently running process;
      iii. sleep for one second;
       iv. Increment dispatcher timer;
        v. Go back to 4.
    5. Exit

*******************************************************************/

#include "hostd.h"

#define VERSION "1.0"

/******************************************************
 
   internal functions
   
 ******************************************************/

char * StripPath(char*);
void PrintUsage(FILE *, char *);
void SysErrMsg(char *, char *);
void ErrMsg(char *, char *);

/* global storage */


/******************************************************/

int main (int argc, char *argv[])
{
    char * inputfile;             // job dispatch file
    FILE * inputliststream;
    PcbPtr inputqueue = NULL;     // input queue buffer
    PcbPtr currentprocess = NULL; // current process
    PcbPtr rrqueue = NULL;        // round-robin queue
	PcbPtr process = NULL;        // working pcb pointer
    int timer = 0;                // dispatcher timer 
	int quantum = QUANTUM;        // current time-slice quantum


//  0. Parse command line

    if (argc == 2) inputfile = argv[1];
    else PrintUsage (stderr, argv[0]);

//  1. Initialize dispatcher queue;
//     (already initialised in assignments above)
    
//  2. Fill dispatcher queue from dispatch list file;
    
    if (!(inputliststream = fopen(inputfile, "r"))) { // open it
          SysErrMsg("could not open dispatch list file: ", inputfile);
          exit(2);
    }

	PcbPtr nextPCB;
    while (!feof(inputliststream)) {  
		// put processes into input_queue
		nextPCB = createnullPcb();
		if(fscanf(inputliststream, "%d, %d, %d, %d, %d, %d, %d, %d", 
				  &(nextPCB->arrivaltime), 
				  &(nextPCB->priority),
				  &(nextPCB->remainingcputime), 
				  &(nextPCB->mbytes),
				  &(nextPCB->req.printers), 
				  &(nextPCB->req.scanners),
				  &(nextPCB->req.modems), 
				  &(nextPCB->req.cds)			)!= 8){
			free(nextPCB);
			continue;
		}
		nextPCB->status = PCB_INITIALIZED;
		inputqueue = enqPcb(inputqueue, nextPCB);
	}
	
//  3. Start dispatcher timer;
//     (already set to zero above)
	
        
//  4. While there's anything in the queue or there is a currently running process:

    while (inputqueue || currentprocess || rrqueue) {
		//unload any pending process from input queue
	//	puts("here");
		
		if(rrqueue){
			puts("STATE OF RR QUEUE:");
			int i = 0;
			PcbPtr j = rrqueue;
			while(i != 5){
				printf("	--"); printPcb(j, stdout);
				i++;
				j = j->next;
			}
		}else {
			puts("rr queue empty");
		}

		
		
		while(inputqueue && inputqueue->arrivaltime <= timer){
			process = deqPcb(&inputqueue);
			process->status = PCB_READY;
			rrqueue = enqPcb(rrqueue, process);
		}
		
		if(currentprocess){
			//if there's a process currently running:
			(currentprocess->remainingcputime)-= quantum;
			
			if((currentprocess->remainingcputime) == 0){
				terminatePcb(currentprocess);
				free(currentprocess);
				currentprocess = NULL;
			}else if(rrqueue){
				//if the process that's running isn't done yet (give a new process a chance):
				suspendPcb(currentprocess);
				rrqueue = enqPcb(rrqueue, currentprocess);
				currentprocess = NULL;
			}
		}
		
		if(!currentprocess && rrqueue){
			//if there is no process running at the moment and there are processes ready and waiting in queue:
			currentprocess = deqPcb(&rrqueue);
			startPcb(currentprocess);
		}
		
		quantum = (currentprocess && (currentprocess->remainingcputime < QUANTUM ))? currentprocess->remainingcputime :
				  !(currentprocess) ? 1 :
				  QUANTUM;
		
		sleep(quantum);
		timer += quantum;
	}
	
   // 5. Exit
    exit (0);
}    


/*******************************************************************

char * StripPath(char * pathname);

  strip path from file name

  pathname - file name, with or without leading path

  returns pointer to file name part of pathname
    if NULL or pathname is a directory ending in a '/'
        returns NULL
*******************************************************************/

char * StripPath(char * pathname)
{
    char * filename = pathname;\

    if (filename && *filename) {           // non-zero length string
        filename = strrchr(filename, '/'); // look for last '/'
        if (filename)                      // found it
            if (*(++filename))             //  AND file name exists
                return filename;
            else
                return NULL;
        else
            return pathname;               // no '/' but non-zero length string
    }                                      // original must be file name only
    return NULL;
}

/*******************************************************
 * print usage
 ******************************************************/
void PrintUsage(FILE * stream, char * progname)
{
    if(!(progname = StripPath(progname))) progname = DEFAULT_NAME;
    
    fprintf(stream,"\n"
"%s process dispatcher (version " VERSION "); usage:\n\n"
"  %s <dispatch file>\n"
" \n"
"  where <dispatch file> is list of process parameters \n\n",
    progname,progname);

    exit(127);
}
/********************************************************
 * print an error message on stderr
 *******************************************************/

void ErrMsg(char * msg1, char * msg2)
{
    if (msg2)
        fprintf(stderr,"ERROR - %s %s\n", msg1, msg2);
    else
        fprintf(stderr,"ERROR - %s\n", msg1);
    return;
}

/*********************************************************
 * print an error message on stderr followed by system message
 *********************************************************/

void SysErrMsg(char * msg1, char * msg2)
{
    if (msg2)
        fprintf(stderr,"ERROR - %s %s; ", msg1, msg2);
    else
        fprintf(stderr,"ERROR - %s; ", msg1);
    perror(NULL);
    return;
}
                                          
