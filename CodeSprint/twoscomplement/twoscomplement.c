#include <stdio.h>
#include <string.h>

int findOnesForNumber(int i);
int findWeight(int i);
int findWeightInef(int i);
long long unsigned findOnes(long n, long m);
long long unsigned findCumulativeWeight(long n);

int main(){
	int runs;
	long a, b;
	fscanf(stdin,"%d",&runs);
	
	while(runs--){
		fscanf(stdin,"%ld %ld",&a, &b);
		printf("%llu\n",findOnes(a,b));
	}
//	
//	long input = atol(argv[1]);
//	long input2 = atol(argv[2]);
//	printf("weight between %ld and %ld: %llu\n", input, input2, findOnes(input, input2));
//	
	
	////////////////////////////////////////////////////
	//Clock testing for individual findWeight algorithms
	//(never actually used)
	////////////////////////////////////////////////////
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

long long unsigned findOnes(long n, long m){
	//the weight of a two's complement number is just the weight of it's opposite (the positive number) but with all the bits flipped and 1 added
	//weight(negativenumber) = 32 - weight(positivenumber)
	
	if(n==0) return findCumulativeWeight(m);
	if(m==0) return (32*(-n) - findCumulativeWeight(~n));
	if(n>0)	return findCumulativeWeight(m) - findCumulativeWeight(n); //subtract weight of n from weight of m
	if(m>0)	return findCumulativeWeight(m) + (32*(-n) - findCumulativeWeight(~n)); //add the weight of m to the weight of n (as a two's complement representation)

	//subtract weight of m from weight of n (each in two's complement representations)	
	return (32*(-n) - findCumulativeWeight(~n)) - (32*(-m) - findCumulativeWeight(~m)) + findOnesForNumber(m);

}

long long unsigned findCumulativeWeight(long n){
	/*
	 Reasoning:
	 Consider all the number up to 2^x in binary.
	 One way to look at the list of numbers up to that power of 2 is that each column has the same number of 1's in it:
	 this has to do with the process of listing all the combinations of bits
	 for a 4-bit number, for instance, there are 1's in the MSB only in the second half of the list. The next bit to the right has 1's every 4 numbers, the next bit has 1's every 2 numbers, and the LSB has 1's every other
	 Therefore, for a power of 2, the total weight of 1's is simply [1 + x * 2^(x-1)]
	 
	 For any other number, we find the next lowest number, but add the difference as well...
	 
	 
	 example:
	 
	 for 13:
	 totalweight = 8 + [(13-8) + totalweight(13-8)]
				 = 8 + [(5)	   + 4 + [(5-4) + totalweight(5-4)]]
	 
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
