#include <stdio.h>
#include <string.h>

/*
 Plan of attack:
 
 DON'T loop through all the consecutive entries
 DON'T loop through every bit of each entry
 
 */

int findOnesForNumber(int i);
int findWeight(int i);

int main(int argc, char *argv[]){
	
	int clock1 = clock();
	
	int i = 0;
	while(i < 100){
		findOnesForNumber(i);
		i++;	
	}
	
	int clock2 = clock();
	
	//*************************************
	
	int clock12 = clock();
	
	i = 0;
	while(i < 100){
		findWeight(i);
		i++;	
	}
	
	int clock22 = clock();
	
	
	//*************************************

	
	int clock13 = clock();
	
	i = 0;
	while(i < 100){
		findWeightInef(i);
		i++;
	}
	
	int clock23 = clock();
		
	printf("findOnesForNumber:	clock = %d\n", clock2-clock1);
	printf("findWeight:		clock = %d\n", clock22-clock12);
	printf("findWeightInef:		clock = %d\n", clock23-clock13);
	
	//	int i = 0;
	//	while(i < 100){
	//		printf("%d----	%d // %d\n", i, findOnesForNumber(i), findWeight(i));
	//		
	//		i++;	
	//	}
	//
	
	return 1;
}



int findWeight(int i){
	if(i == 0) return 0;
	if(i == 1) return 1;
	//if i is a power of 2:
	//reasoning taken from SO question:
	//http://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2	
	if ((i & (i - 1)) == 0) return 1;
	
	//for a number that's greater than 1 and not a power of 2:
	
	//cp = closest power of 2 below i:
	int cp = 2;
	//while the next power of 2 is less than
	while(cp*2 < i) cp = cp*2;
	//...
	return 1 + findWeight(i-cp);
}

int findWeightInef(int i){
	//simulate an action on every bit
	int j = 0;
	int k =0;
	while (j<i) {
		if (j == 5) {
			k++;
		}
		j++;
	}
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
