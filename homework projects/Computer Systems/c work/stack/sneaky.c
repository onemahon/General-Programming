/*
 *  sneaky.c
 *  
 *
 *  Created by Joe on 3/26/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

#include <stdio.h>

//#define M 48
//#define N 12
#define INT_OFFSET 24
#define CHAR_OFFSET 96

void f();
void g();
void h();

int main() {
	char c = 'A';
	int x = 42;
	
	char *cp = &c;
	
	printf("char c:	%c			address of c:	%p\nint x:  %d			address of x:	%p\n", c, cp, x, &x);
	puts("go!");
	f();
	printf("char c:	%c			address of c:	%p\nint x:  %d			address of x:	%p\n", c, &c, x, &x);
}

void f() {
	char naughtChar = 'Z';
	int naughtInt = -100;
	g();
}

void g() {
	char naughtChar = 'Z';
	int naughtInt = -100;
	h();
}

void h() {
	char newC = 'B';
	int newI = 43;
	printf("	char newC:	%c		address of newC:	%p\n	int newI:	%d		address of newI:	%p\n", newC, &newC, newI, &newI);
	
	puts("	INT_OFFSET is 24 and CHAR_OFFSET is 96.");
	puts("	going to work now, be back later...");
	
	char *c = &newC;
	int *i = &newI;
	
	*(c+CHAR_OFFSET) = newC;
	*(i+INT_OFFSET) = newI;
	
	puts("	done!");
}



