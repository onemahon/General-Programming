/*
 *  userp.c
 *  
 *
 *  Created by Joe on 11/23/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */


/*
 USERP operation:
 
 while(counter < 50){
	//request a random page in memory
	choose a random int as the page needed
	set request->page = that random int
	set request->pid = getpid()
	signal mmu (mmu's pid is already known from the original fork)
	sigwait()
	
	get the memory frame number from the request struct
	go to shared memory, get that frame's contents
	print out that frame's contents
	
 }
 
 also need to deal with telling mmu if its writing or just reading from memory
 and if it's going to alter the block then it needs to send it back to shared memory after printing out its contents
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#include "mmu.h"
#include "userp.h"

void runUserP();

void runUserP(int isMMU){
	
	srand(time(NULL));
	
	int count = 0;
	int willMod = 0;
	int pageWanted, realFrame;
	pageStruct request;
	request.pid = getpid();
	request.page = -1;
	request.frame = -1;
	
	pageStruct contents;
	contents.pid = -1;
	contents.page = -1;
	contents.frame = -1;
	contents.willMod = -1;
	contents.randomnum1 = -1;
	contents.randomnum2 = -1;
	strcpy(contents.funfield, "");
	
	
	printf("\n					Frame	||	Page	||	num1	||	num2	||	funfield");
	
	
	int signo;
	sigset_t sigset;
	if ((sigemptyset(&sigset) == -1) ||
		(sigaddset(&sigset, SIGUSR1) == -1) ||
		(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1))
		perror("Failed to block signals before sigwait"); 
	while(count <50){
		willMod = rand()%2; 
		pageWanted = rand()%20; 
		request.page = pageWanted;
		request.willMod = willMod;
		writeShm(0,sizeof(pageStruct),&request);
		fflush(stdout);
		kill(isMMU,SIGUSR1);
		if (sigwait(&sigset, &signo) == -1) {
			perror("Failed to wait using sigwait");
			exit(1);
		}
		
		readShm(0, sizeof(pageStruct), &request);
		realFrame = request.frame;
		//readShm(sizeof(pageStruct)+sizeof(pageStruct)*realFrame, sizeof(pageStruct), &contents);
		readShm(1+realFrame, sizeof(pageStruct), &contents);

		//printf("---	actual contents: frame: %d, page: %d, willmod: %d, pid: %d, randomnum1: %d, randomnum2: %d\n",contents.frame,contents.page,contents.willMod,contents.pid,contents.randomnum1,contents.randomnum2);

		
		//print "contents"
		printf("					%d	||	%d	||	%d	||	%d	||	%s	", realFrame, pageWanted, contents.randomnum1, contents.randomnum2, contents.funfield);// contents.funfield);
		
		if (willMod) {
			//change "contents" struct a bit
			contents.randomnum1 = -1*rand()%100;
			contents.randomnum2 = count;
			strcpy(contents.funfield, "(new 'random' string)");

//			writeShm(sizeof(pageStruct)+sizeof(pageStruct)*realFrame, sizeof(pageStruct), &contents);
			writeShm(1+realFrame, sizeof(pageStruct), &contents);
		}
		
		//see any changes made
		if(willMod)
			printf("\n			(writing)	-------------changed to........	..%d	||	..%d	||	%s	", contents.randomnum1, contents.randomnum2, contents.funfield);// contents.funfield);

		count++;
	}
	
	
}