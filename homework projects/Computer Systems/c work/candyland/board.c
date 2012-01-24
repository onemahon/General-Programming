/*
 *  board.c
 *  
 *
 *  Created by Joe Mahon.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

#include "board.h"
#include <string.h>


gameNode_t * blankNode(){
	gameNode_t *a = (gameNode_t *)malloc(sizeof(gameNode_t));
	a->number = 0;
	a->next = NULL;
	a->prev = NULL;
	a->color = NULL;
	a->op = NULL;
}

gameNode_t * newNode(gameNode_t *n, int nHere, char *color, char *op){
	n->color = (char *)malloc(sizeof(char)*strlen(color));
	strcpy(n->color, color);
	n->op = (char *)malloc(sizeof(char)*strlen(op));
	strcpy(n->op, op);
	n->next = NULL;
	n->prev = NULL; //clear out the next/prev nodes, as there could be some lingerying data there
	n->number = nHere;
	return n;
}

gameNode_t *setPrev(gameNode_t *b, gameNode_t *a){
	b->prev = a;
}

gameNode_t *setNext(gameNode_t *b, gameNode_t *c){
	b->next = c;
}

void killList(gameNode_t *a){
	int c = 0;
	
	
	while (1) {
		if(a->next == NULL)
			break;
		c++;
		free(a->color);
		free(a->op);
		a = a->next;
		free(a->prev);
	} 	
		
	free(a->color);
	free(a->op);
	free(a);
	
	
}