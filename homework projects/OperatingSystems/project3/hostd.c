/*******************************************************************

   hostd

        hostd is a three level feedback 'dispatcher' that reads in a list
        of 'jobs' from a file and 'dispatches' them in feedback mode
        (see below).

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
    4. While there's anything in the queues or there is a currently running process:
        i. Unload any pending processes from the input queue:
           While (head-of-input-queue.arrival-time <= dispatcher timer)
           dequeue process from input queue and enqueue on highest priority feedback
           queue (assigning it the appropriate priority);
       ii. If a process is currently running:
            a. Decrement process remainingcputime;
            b. If times up:
                A. Send SIGINT to the process to terminate it;
                B. Free up process structure memory;
            c. else if other processes are waiting in any of the feedback queues:
                A. Send SIGTSTP to suspend it;
                B. Reduce the priority of the process (if possible) and enqueue it on
                   the appropriate feedback queue;
      iii. If no process currently running && feedback queues are not empty:
            a. Dequeue process from the highest priority feedback queue that is not empty
            b. If already started but suspended, restart it (send SIGCONT to it)
               else start it (fork & exec)
            c. Set it as currently running process;
       iv. sleep for one second;
        v. Increment dispatcher timer;
       vi. Go back to 4.
    5. Exit

*******************************************************************/

#include "hostd.h"

#define VERSION "1.2"
/******************************************************
 
   internal functions
   
 ******************************************************/

int CheckQueues(PcbPtr *);
char * StripPath(char*);
void PrintUsage(FILE *, char *);
void SysErrMsg(char *, char *);
void ErrMsg(char *, char *);

/******************************************************/

int main (int argc, char *argv[])
{
    char * inputfile;             // job dispatch file
    FILE * inputliststream;
    PcbPtr inputqueue = NULL;     // input queue buffer
    PcbPtr fbqueue[N_FB_QUEUES];  // feedback queues
    PcbPtr currentprocess = NULL; // current process
    PcbPtr process = NULL;        // working pcb pointer
    int timer = 0;                // dispatcher timer
    int quantum = QUANTUM;        // current time-slice quantum
    int i;                        // working index

//  0. Parse command line

    if (argc == 2) inputfile = argv[1];
    else PrintUsage (stderr, argv[0]);

//  1. Initialize dispatcher queues;

    for (i = 0; i < N_FB_QUEUES; fbqueue[i++] = NULL);
    
//  2. Fill dispatcher queue from dispatch list file;
    
    if (!(inputliststream = fopen(inputfile, "r"))) { // open it
          SysErrMsg("could not open dispatch list file:", inputfile);
          exit(2);
    }

    while (!feof(inputliststream)) {  // put processes into input_queue
        process = createnullPcb();
        if (fscanf(inputliststream,"%d, %d, %d, %d, %d, %d, %d, %d",
             &(process->arrivaltime), &(process->priority),
             &(process->remainingcputime), &(process->mbytes),
             &(process->req.printers), &(process->req.scanners),
             &(process->req.modems), &(process->req.cds)) != 8) {
            free(process);
            continue;
        }
        process->status = PCB_INITIALIZED;
        inputqueue = enqPcb(inputqueue, process);
    }

//  3. Start dispatcher timer;
//     (already set to zero above)
	
	
	//initialize memory
	MabPtr memall = createnullMab(1024);
	//split memory into "protected" Real Time process memory and User Memory
	
	MabPtr rtmem = memSplit(memall, 64);
	MabPtr mem = rtmem->next;	//work with "mem" as user memory area
	mem->justused = 1; //initialize, the "justused" represents the most recently used, and the algorithm assumes one exists no matter what
	
    PcbPtr feedbackpending = NULL;	// processes waiting for memory
	
	
	
	
//  4. While there's anything in any of the queues or there is a currently running process:

    while (inputqueue || (CheckQueues(fbqueue) >= 0) || currentprocess ) {
//      i. Unload any pending processes from the input queue:
//         While (head-of-input-queue.arrival-time <= dispatcher timer)
//         dequeue process from input queue and and enqueue on feebackpending queue
		
		
        while (inputqueue && inputqueue->arrivaltime <= timer) {
            process = deqPcb(&inputqueue);          // dequeue process
            process->status = PCB_READY;            // set pcb ready
            process->priority = 0;                  // override any priority 
            feedbackpending = enqPcb(feedbackpending, process);
                                                    // & put on queue
        }
		
		
		PcbPtr nextprocess = feedbackpending;
		//unload pending processes from user job queue:
		
		//while (feedbackpending->mbytes is free somewhere in user mem:
		while(feedbackpending && memChk(mem, feedbackpending->mbytes)){
			nextprocess = deqPcb(&feedbackpending); // dequeue process from user job queue
			nextprocess->memoryblock = memAlloc(mem, nextprocess->mbytes); // allocate memory to the process
			nextprocess->priority = 0;
			fbqueue[nextprocess->priority] = enqPcb(fbqueue[nextprocess->priority], nextprocess);
		}
		
		puts("got it");
		

//     iii. If a process is currently running;
        if (currentprocess) {
            currentprocess->remainingcputime -= quantum;
            if (currentprocess->remainingcputime <= 0) {
                terminatePcb(currentprocess);
				
				memFree(currentprocess->memoryblock);
				
                free(currentprocess);
                currentprocess = NULL;
                
//         c. else if other processes are waiting in feedback queues:
            } else if (CheckQueues(fbqueue) >= 0) {
                suspendPcb(currentprocess);
				
                if (++(currentprocess->priority) >= N_FB_QUEUES)
                    currentprocess->priority = N_FB_QUEUES - 1;
                fbqueue[currentprocess->priority] = 
                    enqPcb(fbqueue[currentprocess->priority], currentprocess);
                currentprocess = NULL;
            }
        }
        
//    iv. If no process currently running && feedback queues are not empty:

        if (!currentprocess && (i = CheckQueues(fbqueue)) >= 0) {
            currentprocess = deqPcb(&fbqueue[i]);
			startPcb(currentprocess);
        }
        
//      v. sleep for quantum;

        quantum = currentprocess && currentprocess->remainingcputime < QUANTUM ?
                  currentprocess->remainingcputime :
                  !(currentprocess) ? 1 : QUANTUM;
        sleep(quantum);
		
        timer += quantum;
    }
	
    exit (0);
}    


/*******************************************************************

int CheckQueues(PcbPtr * queues)

  check array of dispatcher queues

  return priority of highest non-empty queue
          -1 if all queues are empty
*******************************************************************/
int CheckQueues(PcbPtr * queues)
{
    int n;

    for (n = 0; n < N_FB_QUEUES; n++)
        if (queues[n]) return n;
    return -1;
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
                                          
