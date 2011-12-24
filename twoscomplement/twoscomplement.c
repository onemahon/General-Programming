#include <stdio.h>
#include <string.h>

/*
 Plan of attack:
 
 DON'T loop through all the consecutive entries
 DON'T loop through every bit of each entry
 
 
 
 
 
 
 
 
 
 
 
 */

int findOnesForNumber(int i);

int main(int argc, char *argv[]){
	
	int i = 0;
	while(i < 100){
		printf("%d----	%d\n", i, findOnesForNumber(i));
		
		i++;	
	}
	
	return 1;
}


int findOnesForNumber(int i)
{
	//Hamming Weight: 
	//References:
	//	
	//http://graphics.stanford.edu/~seander/bithacks.html
	//	
	//http://en.wikipedia.org/wiki/Hamming_weight
	//	
	//http://gurmeetsingh.wordpress.com/2008/08/05/fast-bit-counting-routines/
	//	
	//http://aggregate.ee.engr.uky.edu/MAGIC/#Population%20Count%20(Ones%20Count)
	//
	//borrowed from: 
	//http://stackoverflow.com/questions/109023/best-algorithm-to-count-the-number-of-set-bits-in-a-32-bit-integer
	
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
