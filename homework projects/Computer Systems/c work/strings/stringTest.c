/*
 *  stringTest.c
 *  
 *
 *  Created by Joe on 3/15/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */


#include "myString.c"
#include <stdio.h>
#include <stdlib.h>

int main(){
	char *s1 = fetchString();
	char *s2 = fetchString();
	
	int l = strlength(s1);
	
	printf("-----\n");
	printf("length of first string: %d\n", l);
	printf("-----\n");
	
	int num = countChars(s1, 's');
	
	printf("number of 's's in first string: %d\n", num);
	printf("-----\n");
		
	int res = compare(s1, s2);
	
	printf("compare result: %d\n", res);

	
	if(res > 0)
		puts("first word comes before second word");
	else if(res < 0)
		puts("second word comes before first word");
	else
		puts("both strings are equal");
	printf("-----\n");

	char *conc = frankenString(s1, s2);
	
	printf("concatenated strings: %s\n", conc);
	printf("-----\n");

	
	char *remChar = removeChar('s', s1);
	
	printf("first string minus s's: %s\n", remChar);
	printf("-----\n");
	
		
	char c = favChar(s1);
	printf("favorite character in first string: %c\n", c);
	printf("-----\n");
	
	free(remChar);
	free(conc);
	free(s1);
	free(s2);
	
	return 0;
}