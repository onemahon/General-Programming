/*
 *  sneaky-partB-2011.c
 *  
 *
 *  Created by Joe on 3/26/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>

#define INT_OFFSET 12
#define CHAR_OFFSET 48

void f();

int main() {
	char c[8];
	int x, y;
	y = 42;
	char new = 'A';
	for(x = 0; x<8; x++){  //fill the c array with A forwards
		c[x] = new;
		printf("c[%d]:	%c		address of c[%d]:	%p\n", x, c[x], x, &c[x]);
		new++;
	}
	printf("Y: y = %d\n", y);
	puts("----");
	f();
	puts("NOW the C array contains:");
	for(x = 0; x<8; x++){  //fill the c array with A forwards
		printf("c[%d]:	%c		address of c[%d]:	%p\n", x, c[x], x, &c[x]);
	}
	printf("Y: y = %d\n", y);
}

void f() {
	char c1[8];
	int x1, y1;
	
	y1 = 2000;
	
	puts("    f(): ");

	//fill the array with replacement values and determine offsets
	int p = 0;
	char new = 'Z';
	c1[0] = 'L';
	c1[1] = 'O';
	c1[2] = 'V';
	c1[3] = 'E';
	c1[4] = '2';
	c1[5] = 'E';
	c1[6] = 'A';
	c1[7] = 'T';
	
	for(p = 0; p < 8; p++){
		printf("	c1[%d]:	%c		address of c1[%d]:	%p\n", p, c1[p], p, &c1[p]);
	}
	printf("	Y1: y1 = %d\n", y1);
	
	puts("	INT_OFFSET is 12, while CHAR_OFFSET is 48\n----");
	
	char *cp;
	//go through main to replace each of the original array values
	for (x1 = 0; x1 < 8; x1++){
		cp = &c1[x1];
		*(cp+CHAR_OFFSET) = c1[x1];
	}
	
	int *ip = &y1;
	*(ip+INT_OFFSET) = y1;
}




