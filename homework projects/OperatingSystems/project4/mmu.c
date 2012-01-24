/*
 *  mmu.c
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

#define MAX 80
/*
struct request{
	int pid;
	int page;
	int frame;
	int willMod;
};
typedef struct request req;

struct pageTableEntry{
	int frame;
	int valid;
	int referenced;
	int modified;
};
typedef struct pageTableEntry pte;

struct pageContents{
	int randomnum1;
	int randomnum2;
	char funfield[80];
};
typedef struct pageContents pageStruct;

void runMMU();
*/

/*
 MMU operation:
 upon starting process, sigwait for signal from userP
 when it gets the signal, check the request portion of Shared Mem for what page that process is looking for
 check the PT for that page (by cycling through the pt array until (ptr->page == pagenum from req)
	--> after the loop, ptr is pointing at the correct page in the page table (since all pages are there)
 if ptr->valid == 1 {
	// page is already in memory: 
	set request->frame to ptr->frame
	signal the userP
	sigwait
 }else{
	//page needs to be added to memory
	//replacement algorithm:
	****FCFS: keep a list of when the pages arrived... i dunno do something cool here to replace a page
	****LRU: figure this out as well...
	****my own designed algorithm: some kind of combination???
	
	well, once a victim is selected:
	set its pte's "valid" bit to 0
	copy all of its values back onto the disk array
	
	copy new page requested by user process into that same slot in shared mem
	set new page's PTE's valid bit to 0, modified to 0, and referenced to 1, and frame to the victim's old frame number
	
	so now it seems that the process has correctly been replaced... make sure all the details are correct, then:
	set request->frame to current frame of memory
	signal the userp
	sigwait
 }
 */


#include "mmu.h"

#define FIRSTCOME 1
#define LEASTRECENT 2

void runMMU(int repalg){
	
	srand(time(NULL));
	
	// Generate "pagetable" array
	pte pagetable[20];
	int i = 0;
	pageptr * head;
	pageptr * prev;
	pageptr * cur;
	

	while(i < 20){
		pagetable[i].frame = 0;
		pagetable[i].valid = 0;
		pagetable[i].referenced = 0;
		pagetable[i].modified = 0;
		
		//generate list of pteptrs to keep track of LRU page
		cur = (pageptr *)malloc(sizeof(pageptr));
		cur->page = i;
		cur->next = NULL;
		
		if (i == 0) {
			head = cur;
		}else {
			prev->next = cur;
			
		}
		prev = cur;

		i++;
	}
		
	// Generate "disk" array
	pageStruct disk[20];
	i = 0;
	printf("created: ");
	while(i<20){
		printf("%d... ", i);
		disk[i].randomnum1 = rand()%100;
		disk[i].randomnum2 = rand()%100;
		strcpy(disk[i].funfield, "(initial string)");
		
		//these are never used on disk...
		disk[i].pid = -1;
		disk[i].page = -1;
		disk[i].frame = -1;
		disk[i].willMod = -1;
		//fprintf(stdout, "page %d contains rn1 %d and rn2 %d\n", i, disk[i].randomnum1, disk[i].randomnum2);
		//writeShm(1+i, sizeof(pageStruct), &disk[i]);
		i++;
	}
	printf("frames of memory.\n");
	
	
	//initialize array to keep track of empty frames in memory
	int memvalid[10];
	int m = 0;
	while (m<10) {
		memvalid[m] = -1;
		m++;
	}
	
	pageStruct request;
	request.pid = -1;
	request.page = -1;
	request.frame = -1;
	request.willMod = -1;
	request.randomnum1 = -1;
	request.randomnum2 = -1;
	strcpy(request.funfield, "nullstring");
	//initialize to -1, shouldn't remain that way after anything requests a new block
	pageStruct swap;
	swap.pid = -1;
	swap.page = -1;
	swap.frame = -1;
	swap.willMod = -1;
	swap.randomnum1 = -1;
	swap.randomnum2 = -1;	
	strcpy(swap.funfield, "nullstring");
	int page, frame, pid;
	int victim = -1;
	
	int signo;
	sigset_t sigset;
	if ((sigemptyset(&sigset) == -1) ||
		(sigaddset(&sigset, SIGUSR1) == -1) ||
		(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1))
		perror("Failed to block signals before sigwait"); 
	
	while(1){
		if (sigwait(&sigset, &signo) == -1) {
			perror("Failed to wait using sigwait");
			exit(1);
		}

		//get the input struct from the userP input pipe
		readShm(0,sizeof(pageStruct),&request);
		page = request.page;
		
		
		//move that page to the back end of the "used" queue (called "head", here)
		pageptr *p = head;
		pageptr *prev = NULL;
		while (p->page != page && p->next != NULL) {
			prev = p;
			p = p->next;
		}
		if (p->next == NULL && p->page != page) {
			puts("\n\nerror finding the page in the 'referenced' queue");
			printf(" when looking for page %d", page);
		}
		
		pageptr * last = head;
		while (last->next != NULL) {
			last = last->next;
		}
		
		//p now points to that struct that needs to be taken out
		if (p->next == NULL) {
			//already last struct
		}else if (prev == NULL) {
			//first struct
			head = p->next;
			last->next = p;
			p->next = NULL;
		}else {
			//any other struct
			prev->next = p->next;
			p->next = NULL;
			last->next = p;
			//printf("\nmoved page %d to end\nlinked page %d to following page %d", p->page, prev->page, (prev->next)->page);
		}
		
		
		//placed the struct containing the page being called at the end of the "referenced" queue
		//thus the leftmost struct whose page is valid is the LRU page at any given time
		
		//find out where request.page is located based on page table
		if(pagetable[page].valid){			
			// page is already in memory
			printf("\n_____________________________________________________________________________________________________________");
			printf("\nNo page fault.\n");
			request.frame = pagetable[page].frame;
			writeShm(0,sizeof(pageStruct),&request);
			
			//update page table accordingly
			pagetable[page].referenced += 1;
			if (request.willMod) {
				//update the PTE for this page as being modified recently
				pagetable[page].modified += 1;
			}
			
			kill(request.pid,SIGUSR1);
		} else {
			// find victim, add page to memory
			printf("\n_____________________________________________________________________________________________________________");
			printf("\nPage fault: find a victim from memory.\n");
			
			//check for empty slots in shared memory
			int j = 0;
			while (j < 10 && memvalid[j] != -1) {
				j++;
			}

			if (j < 9) {
				//j is empty, use it
				victim = j;
			}else if (j == 9 && memvalid[j] == -1) {
				//9 is empty, use it
				victim = 9;
			}else {
				//none are empty, find a victim
				
				//pages are arranged in order they came in
				//FCFS: keep track of the previously victimized page, take the next one as the victim.
				
				//read in victim page from memory
				//copy its contents into disk
				//read out disk into shared memory
				
				//my own:
				//check the valid, referenced, and modified bits in the pte:
				//  cycle through the array: once you find a valid pte who has not been referenced or modified, choose it immediately
				//  as you cycle through, set any other "referenced" bits to 0
				
				//my own:
				//make "referenced" an int instead of a bit
				//

				
				
				
				//FCFS
				if(repalg == FIRSTCOME){
					//first come first serve
					victim++;
					if (victim == 10) {
						victim = 0;
					}
				}else if(repalg == LEASTRECENT){
					
					//LRU
					pageptr * ptr = head;
					while(!(pagetable[ptr->page].valid) && ptr->next != NULL){
						ptr = ptr->next;
					}
					if(ptr->next == NULL){
						puts("error in the replacement algorithm.");
					}
					//ptr now contains the page of the victim, so: 
					victim = pagetable[ptr->page].frame;
				}else{
					//my own algorithm
					while (1) {
						int index = rand()%20;
						if (pagetable[index].valid == 1) {
							victim = pagetable[index].frame;
							break;
						}
					}
					
					
					
					/*
					 find victim in my own way....
					 
					 
					 */
					
					
					
				}
				
				readShm(1+victim, sizeof(pageStruct), &swap);

				// memvalid[victim] represents actual page number:
				disk[memvalid[victim]].randomnum1 = swap.randomnum1;
				disk[memvalid[victim]].randomnum2 = swap.randomnum2;
				strcpy(disk[memvalid[victim]].funfield, swap.funfield);
				
				pagetable[memvalid[victim]].valid = 0;
				
			}
			
			//update memvalid array to contain new page's index
			memvalid[victim] = page;
			pagetable[page].frame = victim;

			// copy the requested page into the "memory" array
			swap.randomnum1 = disk[page].randomnum1;
			swap.randomnum2 = disk[page].randomnum2;
			strcpy(swap.funfield, disk[page].funfield);
			writeShm(1+victim, sizeof(pageStruct), &swap);

			// update the request
			request.frame = pagetable[page].frame;
			writeShm(0,sizeof(pageStruct),&request);

			//update page table accordingly
			pagetable[page].referenced = 1;
			pagetable[page].valid = 1;
			if (request.willMod) {
				//update the PTE for this page as being modified recently
				pagetable[page].modified = 1;
			}
			
			kill(request.pid,SIGUSR1);
		}
	}
	
}