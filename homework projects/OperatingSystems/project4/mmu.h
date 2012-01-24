/*
 *  mmu.h
 *  
 *
 *  Created by Joe on 11/23/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

struct pageTableEntry{
	int frame;
	int valid;
	int referenced;
	int modified;
};
typedef struct pageTableEntry pte;

struct pageContents{
	//page data
	char funfield[80];
	int randomnum1;
	int randomnum2;
	
	//request metadata (only used in slot 0)
	int pid;
	int page;
	int frame;
	int willMod;
};
typedef struct pageContents pageStruct;

struct pagePtr{
	int page;
	struct pagePtr * next;
};
typedef struct pagePtr pageptr;
typedef struct pagePtr * pageptrptr;

void runMMU();
