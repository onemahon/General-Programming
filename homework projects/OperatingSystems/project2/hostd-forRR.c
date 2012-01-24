/*******************************************************************

HOST dispatcher 

    hostd

        hostd is a round-robin 'dispatcher' that reads in a list of 'jobs'
        from a file and 'dispatches' them in round-robin mode (see below).

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
    4. While there's anything in any of the queues or there is a currently running process:
        i. Unload any pending processes from the input queue:
           While (head-of-input-queue.arrival-time <= dispatcher timer)
           dequeue process from input queue and enqueue on RR queue;
       ii. If a process is currently running:
            a. Decrement process remainingcputime;
            b. If times up:
                A. Send SIGINT to the process to terminate it;
                B. Free up process structure memory;
            c. else if other processes are waiting in RR queue:
                A. Send SIGTSTP to suspend it;
                B. Enqueue it back on RR queue;
      iii. If no process currently running && RR queue is not empty:
            a. Dequeue process from RR queue
            b. If already started but suspended, restart it (send SIGCONT to it)
               else start it (fork & exec)
            c. Set it as currently running process;
       iv. sleep for one second;
        v. Increment dispatcher timer;
       vi. Go back to 4.
    5. Exit

*******************************************************************/

#include "hostd.h"

#define VERSION "1.1"
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
    PcbPtr rrqueue = NULL;        // round-robin queue
    PcbPtr currentprocess = NULL; // current process
    PcbPtr process = NULL;        // working pcb pointer
    int timer = 0;                // dispatcher timer
    int quantum = QUANTUM;        // current time-slice quantum

//  0. Parse command line

    if (argc == 2) inputfile = argv[1];
    else PrintUsage (stderr, argv[0]);

//  1. Initialize dispatcher queue;
//     (already initialised in assignments statements above above)
    
//  2. Fill dispatcher queue from dispatch list file;
    
    if (!(inputliststream = fopen(inputfile, "r"))) { // open it
          SysErrMsg("could not open dispatch list file:", inputfile);
          exit(2);
    }

    while (!feof(inputliststream)) {  // put processes into input_queue
// your code goes here
// don't forget to initialize the PCB status
// and put PCB on the queue
    }

//  3. Start dispatcher timer;
//     (already set to zero above)
        
//  4. While there's anything in any of the queues or there is a currently running process:

    while (inputqueue || rrqueue || currentprocess ) {// i'll give you this outer loop, now do the rest

//      i. Unload any pending processes from the input queue:
//         While (head-of-input-queue.arrival-time <= dispatcher timer)
//         dequeue process from input queue and enqueue on RR queue
//     ii. If a process is currently running;
//          a. Decrement process remainingcputime;            
//          b. If times up:                
//             A. Send SIGINT to the process to terminate it;
//             B. Free up process structure memory
//         c. else if other processes are waiting in RR queue:
//             A. Send SIGTSTP to suspend it;                
//             B. Enqueue it back on RR queue;
//    iii. If no process currently running && RR queue is not empty 
//         a. Dequeue process from RR queue            
//         b. If already started but suspended, restart it (send SIGCONT to it)
//              else start it (fork & exec)
//         c. Set it as currently running process;
//      iv. sleep for quantum;            
//       v. Increment dispatcher timer;            
//      vi. Go back to 4.
}
//    5. Exit
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
                                          
