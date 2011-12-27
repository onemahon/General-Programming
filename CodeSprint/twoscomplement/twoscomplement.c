#include <stdio.h>
#include <string.h>

/*
 Plan of attack:
 
 DON'T loop through all the consecutive entries
 DON'T loop through every bit of each entry
 
 */

int findOnesForNumber(int i);
int findWeight(int i);
int findWeightInef(int i);
long long unsigned findCumulativeWeight(long unsigned n);

int main(int argc, char *argv[]){
	
	long unsigned input = atol(argv[1]);
	printf("ones up to %lu: %llu\n", input, findCumulativeWeight(input));
	//
//	int clock1 = clock();
//	
//	int i = 0;
//	while(i < 100){
//		findOnesForNumber(i);
//		i++;	
//	}
//	
//	int clock2 = clock();
//	
//	//*************************************
//	
//	int clock12 = clock();
//	
//	i = 0;
//	while(i < 100){
//		findWeight(i);
//		i++;	
//	}
//	
//	int clock22 = clock();
//	
//	
//	//*************************************
//
//	
//	int clock13 = clock();
//	
//	i = 0;
//	while(i < 100){
//		findWeightInef(i);
//		i++;
//	}
//	
//	int clock23 = clock();
//		
//	printf("findOnesForNumber:	clock = %d\n", clock2-clock1);
//	printf("findWeight:		clock = %d\n", clock22-clock12);
//	printf("findWeightInef:		clock = %d\n", clock23-clock13);
//	
//	//	int i = 0;
//	//	while(i < 100){
//	//		printf("%d----	%d // %d\n", i, findOnesForNumber(i), findWeight(i));
//	//		
//	//		i++;	
//	//	}
//	//
	
	return 1;
}



/*
 
 if we consider any number:
 the total weight from zero is going to include to total weight from zero of the second next lowest power of two, that multiplied by two, plus the weightb between that next lowest power of 2 and the number itself.
 
 for a number N:
 find the weight from 0 to the 2nd highest power of two
 (i.e. for 15, find weight from 0 to 4)
 double that weight, add that number
 (i.e. double weight from 0-4, add 4 --> represents the extra 1 from 4 to 8)
 now add the weight from the next highest power of 2 to N
 (i.e. add weight from 8 to 15)
 now this is still the slow step when we get to high numbers - because it only cuts just less than half of the work out in worst case scenarios
 
 
 
 
 
 0001 1  1
 0010 2  2
 0011 3  4
 0100 4  5
 0101 5  7
 0110 6  9
 0111 7  12
 1000 8  13
 1001 9  
 1010 10 
 1011 11 
 1100 12 
 1101 13 
 1110 14 
 1111 15 
 
 */


long long unsigned findCumulativeWeight(long unsigned n){
	
	
	/*
	 any power of 2's total weight:
	 
	 2^6 = 64
	 totalWeight(2^x) = 1 + x*2^(x-1)
	 
	 any number's total weight:
	 
	 the next lowest power of 2's weight + (the weight of the difference...)
	 the weight of the difference will be n+(the weight of the next lowest power of 2's cumulative weight)
	 
	 
	 
	 i.e.
	 for 13:
	 
	 totalweight = 8 + [(13-8) + totalweight(13-8)]
				 = 8 + [(5)	   + 4 + [(5-4) + totalweight(5-4)]]
	 
	 so totalweight(n) = next lowest power of 2 + n-(
	 
	 so for totalweight(n):
	 
	 int x = next lowest power of 2
	 weight = x + (n-x) + totalweight(n-x)
	 
	 
	 */
	
	if(n == 0) return 0; //base cases
	if(n == 1) return 1;
	if(n == 2) return 2;
	
	long long unsigned total = 0;
	long unsigned p = 1; //next lowest power of 2 under n
	int i = 0;
	while (2*p <= n) {
		i++;
		p = 2*p;
	}
	//weight of p:
	long unsigned w = 1 + i*(p/2);
	
	//2 = 2^1
	//64 = 2^6
	
	//if n is a power of 2:
	if ((n & (n - 1)) == 0) {
		return w;
	}
	
	
	return w + (n-p) + findCumulativeWeight(n-p);
	
	/*
	 Reasoning:
	 Consider all the number up to 2^x in binary.
	 One way to look at the list of numbers up to that power of 2 is that each column has the same number of 1's in it:
		this has to do with the process of listing all the combinations of bits
		for a 4-bit number, for instance, there are 1's in the MSB only in the second half of the list. The next bit to the right has 1's every 4 numbers, the next bit has 1's every 2 numbers, and the LSB has 1's every other
	 Therefore, for a power of 2, the total weight of 1's is simply [1 + x * 2^(x-1)]
	 
	 For any other number, we find the next lowest number, but add the difference as well (which is 
	 
	 
	 
	 */
	
	
	
	
	
	
//	
//	int total = 0;
//	while (m <= n) {
//		//if m is even, we're adding twice the weight + 1 for the next odd number
//		//if m is odd, we're adding twice the weight - 1 for the previous even number
//		
//		//if m is even and n is odd, m finishes on the next lowest even number (and so the tally is correct)
//		//if m is even and n is even, m needs to subtract 1
//		//if m is odd and n is odd, m finishes on n itself, has counted the correct tally
//		//if m is odd and n is even, m finishes on the next lowest odd number, and so needs to 
//		
//		total += 2*findOnesForNumber(m);
//		m += 2;
//	}
//	
	
	
	
	//
//	
//	int n = 0;
//	int totalOnes = 0;
//	if (N%2 == 0) {
//		while (n<N) {
//			printf("adding weight of %d\n", n);
//			totalOnes += 2*findOnesForNumber(n-1)+1;
//			n+=2;
//		}
//	}else {
//		while (n<=N) {
//			totalOnes += 2*findOnesForNumber(n-1)+1;
//			n+=2;
//		}
//	}
//	return totalOnes;
	
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
