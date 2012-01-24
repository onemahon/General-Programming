/*
 *  project4.c
 *  
 *
 *  Created by Joe on 11/23/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "ipcshm.h"
#include "mmu.h"

int main(){
	
	int isUserP, isMMU;
	//fork off the MMU
	//fork off the user process
	
	puts("Enter '1' for FCFS, '2' for LRU, and '3' for my own algorithm.");
	int type;
	fscanf(stdin, "%d", &type);

	//create shared memory that contains all the data that user might request
	//create a small block of shared memory for requests and responses
	int sharedmem = createShm(sizeof(pageStruct)*21);
	printf("sharedmem: %d\n", sharedmem);
	
	if (isMMU = fork()) {
		sleep(1);
		if (isUserP = fork()) {
			//main process that knows UserP's pid
			int res;
			waitpid(isUserP,&res, 0);
			puts("\ndone.");
			kill(isMMU, SIGKILL); //kill MMU once user is done running
			destroyShm();
		}else {
			runUserP(isMMU);
		}
	}else {
		runMMU(type);
	}
}