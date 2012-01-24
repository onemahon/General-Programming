#ifndef MAB_H
#define MAB_H

/*
 *  mab.h
 *  
 *
 *  Created by Joe on 11/2/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

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


#define UNALLOCATED 0
#define ALLOCATED 1

#define FIRSTFIT 1
#define BESTFIT 2
#define WORSTFIT 3
#define NEXTFIT 4


struct mab { 
	int offset;
	int size; 
	int allocated; 
	struct mab * next; 
	struct mab * prev;
	int justused;
};
typedef struct mab Mab; 
typedef Mab * MabPtr;

MabPtr createnullMab(int size);
MabPtr memChk(MabPtr m, int size, int alg);	// check if memory available 
MabPtr memAlloc(MabPtr m, int size, int alg); // allocate memory block 
MabPtr memFree(MabPtr m);	// free memory block
MabPtr memMerge(MabPtr m);	// merge two memory blocks 
MabPtr memSplit(MabPtr m, int size); // split a memory block

#endif
