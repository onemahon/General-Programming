/*
 *  candyland.c
 *  
 *
 *  Created by Joe Mahon.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

#include "board.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	//open the file and go through it to read all the data into a linked list
	FILE *fred = fopen("board", "r");
	
	char buffer[80];
	
	if(fred == NULL){
		puts("file not found");
		exit(1);
	}
	
	gameNode_t *a;
	a = blankNode();
	//int n = 0;
	char *c, *o, *m; //m, char and op, to hold temp info about the new Node
	int num = 0;
	
	gameNode_t *prev;
	gameNode_t *beg;
	
	beg = a;
	
	int count = 0;
	
	while(1){
		fgets(buffer, 80, fred);
		if(feof(fred))
			break;
		c = strtok(buffer, " ");
		o = strtok(NULL, " ");
		m = strtok(NULL, "\n");
		
		num = atoi(m);
		
		newNode(a, num, c, o);


		if(prev!=NULL){
			setPrev(a, prev);
			setNext(prev, a);
		}
		
		prev = a;
		
		a = blankNode();

		count++;
	}
	puts("The linked list board should (fingers crossed) be complete now");
	puts("Ready, go!\n");
	
	
	//run the game 5 times
	int games = 0;
	int counterSum = 0;
	srand (time(NULL));		

	while(games<5){
		
		gameNode_t *me = beg;
		gameNode_t *save;
		int myMove;
		int counter = 0;
		
		while(strcmp(me->op, "WIN") != 0){			
			myMove = rand()%4+1;
			if(myMove == 1){
				puts("selected RED");
				save = me;
				if(me != beg)
					me = me->next;
				
				while(strcmp(me->color, "RED") != 0){
					if(me->next != NULL){
						me = me->next;
					}else {
						me = save;
						break;
					}
				}
			}
			if(myMove == 2){
				puts("selected BLUE");
				save = me;
				me = me->next;
				
				while(strcmp(me->color, "BLUE") != 0){
					if(me->next != NULL){
						me = me->next;
					}else {
						me = save;
						break;
					}
				}
			}
			if(myMove == 3){
				puts("selected ORANGE");
				save = me;
				me = me->next;
				
				while(strcmp(me->color, "ORANGE") != 0){
					if(me->next != NULL){
						me = me->next;
					}else {
						me = save;
						break;
					}
				}
			}
			if(myMove == 4){
				puts("selected GREEN");
				save = me;
				me = me->next;
				
				while(strcmp(me->color, "GREEN") != 0){
						me = me->next;
				}
			}
			
			if(strcmp(me->op, "GOBACK") == 0){
				int count = 0;
				int offset = me->number;
				printf("oh, dang! go back %d\n", offset);
				while(count<offset){
					me = me->prev;
					count++;
				}
			}
			if(strcmp(me->op, "GOFOWARD") == 0){
				int count2 = 0;
				int offset2 = me->number;
				printf("lucky you! go forward %d\n", offset2);
				while(count2<offset2){
					me = me->next;
					count2++;
				}
			}
			counter++; 
		}
				
		puts("\nHey, congrats! You won! I bet you're pretty proud of yourself.");
		printf("Well, it only took you %d turns to do it. Slowpoke.\n", counter);
		puts("\n");
		
		counterSum = counterSum + counter;
		games++;
	}
	
	int avg = counterSum/5;

	puts("*******************");
	printf("Your marvelous average to finish is ... %d turns\n\n", avg);

	killList(beg);
	fclose(fred);
}







