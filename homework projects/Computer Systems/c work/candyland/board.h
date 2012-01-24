/*
 *  board.h
 *  
 *
 *  Created by Joe Mahon.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct gameNode{
	struct gameNode *prev;
	struct gameNode *next;
	char *op;
	char *color;
	int number;
	
}gameNode_t;

gameNode_t * newNode(gameNode_t *node, int number, char *color, char *op); //will build the node, given certain characteristics
gameNode_t * addPrev(gameNode_t *b, gameNode_t *a);
gameNode_t * addNext(gameNode_t *b, gameNode_t *c);

void killList(gameNode_t *a);


