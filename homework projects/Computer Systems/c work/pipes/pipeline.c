/*
 *  pipeline.h
 *  Joe Mahon
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#define MAX 80

void childA();
void childB();
void childC();
void parent();
char *fetchString();
void error_exit(char * s);

//pipe X is between A and B child, pipe Y is between B and C
int pipeXa[2];
int pipeXb[2];
int pipeYb[2];
int pipeYc[2];

//child ids
int pidA;
int pidB;
int pidC;

int main(){
	
	if(pipe(pipeXa) == -1)	
		error_exit("pipe() failed");
	if(pipe(pipeXb) == -1)
		error_exit("pipe() failed");
	if(pipe(pipeYb) == -1)	
		error_exit("pipe() failed");
	if(pipe(pipeYc) == -1)	
		error_exit("pipe() failed");
	
	pidA = fork(); // now parent has pid of A, child A has 0 in pidA var
	if(pidA == 0){
		//prevent child A from forking
	}else{
		//this section only performed in the parent node, while pidA is not 0
		pidB = fork(); // now parent has pid of B, child B has pid of A in var pidA and 0 in var pidB
		if(pidB == 0){
			//prevent child B from forking
		}else{
			//this section only performed in the parent node, while pidA wasn't 0 and pidB is not 0
			pidC = fork(); // now parent has pid of C, child C has pid of B and A in var pidB and pidA and 0 in var pidC
		}
	}
	
	if(pidA == 0)
		childA(); //string getter
	else if(pidB == 0)
		childB(); //string transformer
	else if(pidC == 0)
		childC(); //string writer

	parent(); //terminator
}

void parent(){
	int s1, s2, s3;
	
	waitpid(pidA, &s1, 0);
	waitpid(pidB, &s2, 0);
	waitpid(pidC, &s3, 0);
	//waits for all three processes to be finished
	exit(0);
}



void childA(){
	close(pipeXa[0]);
	close(pipeXb[1]);
	//A is not reading from B, and B is not reading from A really - they're writing at each other
	
	//get the string entered in Terminal
	char *res = fetchString();
	printf("Child A got %s from command line \n", res);

	while(strcmp(res, "quit") != 0){
		printf("A is writing a message to B\n");
		//active pipeline -- writing and waiting for a notification back from process B
		write(pipeXa[1],res,MAX); //child A writes the string to its shared pipe on the write side, where B will pick it up

		char *notification = (char *)malloc(sizeof(char)*4);
		read(pipeXb[0], notification, 4); //read the notification from childB's buffer, it will write it in whenever it is all set - until then, child 1 is blocked here
		while(strcmp(notification, "done")!=0){
			//sleep(1);
			read(pipeXb[0], notification, 4); //this is the actual loop that shows the blocking
			if(strcmp(notification, "done") == 0)
				printf("ERROR--->there's an issue with child B's notification, godspeed. \n");
		}
		res = "";
		printf("A just got a notification from B\n");
		res = fetchString(); //go get the next string from the command line
		printf("Child A got %s from command line \n", res);
	}
	
	write(pipeXa[1], "quit", 4);
	
	printf("Child A closing its pipes.\n");
	close(pipeXa[1]);
	close(pipeXb[0]);
	
	exit(0);
}

void childB(){
	close(pipeXa[1]);
	close(pipeXb[0]);
	
	close(pipeYb[0]);
	close(pipeYc[1]);
	
	//printf("pid of A: %d, pid of B: %d, pid of C: %d\n", pidA, pidB, pidC);

	//read in from child a
	//send notification to child a
	//transform this string
	//send to child c
	//wait for a done notification from child c
	//read in another line from child a
	//...continue until child a's line reads in as "quit"

	char *readIn;
	char * edits;
	while(strcmp(readIn, "quit") != 0){
		readIn = (char *)malloc(sizeof(char)*MAX);
		read(pipeXa[0], readIn, MAX); //wait for A to send a string this way, and put it in var readIn		
		printf("Child B got %s from the pipe from child A\n", readIn);

		printf("B is notifying A\n");
		write(pipeXb[1], "done", 4); //notify child A that we got it's string

		edits = (char *)malloc(sizeof(char)*strlen(readIn));
		strcpy(edits, readIn);
		char *ptr = edits;
		//actual transformation technique
		int i;
		for(i=0; i<strlen(edits); i++){
			if(isupper(*(ptr+i)))
				*(ptr+i)=tolower(*(ptr+i));
			else {
				*(ptr+i)=toupper(*(ptr+i));
			}
		}
		write(pipeYb[1], edits, MAX); //once all set, write the resulting string to the write-out pipe for child B
		
		char *notification2 = (char *)malloc(sizeof(char)*4);
		read(pipeYc[0], notification2, 4); //wait for the notifcation to come in that child C is all done before reading the next line in from child A's write-out pipe
		while(strcmp(notification2,"done")!=0){
			sleep(1); //just waiting until child C writes "done" into var pipeYc
			read(pipeYc[0],notification2,4); //checks what's written into the pipe continuously until it says "done"
		}
		printf("B just got a notification from C\n");
		free(readIn);
	}
	
	write(pipeYb[1], "quit", 4);
	
	close(pipeYb[1]);
	close(pipeYc[0]);

	close(pipeXa[0]);
	close(pipeXb[1]);
	
	exit(0);
}

void childC(){
	close(pipeYb[1]);
	close(pipeYc[0]);
	
	//printf("pid of A: %d, pid of B: %d, pid of C: %d\n", pidA, pidB, pidC);

	FILE *file;
	file = fopen("pipesResults","w"); //writing to an external file
	char *input = (char *)malloc(sizeof(char)*MAX);
	while(strcasecmp(input,"quit")!=0){
		read(pipeYb[0],input,MAX); //read from child B
		printf("Child C got %s from the pipe from child B\n", input);
		
		fputs(input, file);
		printf("C is notifying B\n");
		write(pipeYc[1], "done\0", 4); //notify child B that writing to File has been done
	}
	fclose(file);
	puts("output file \"pipesResults\" created. Closing");
	
	close(pipeYb[0]);
	close(pipeYc[1]);
	
	exit(0);
}

char * fetchString(){
	char * s = (char *)malloc(sizeof(char));
	char c;
	
	int i=0;
	c = getchar();
	while(c != '\n'){
		*(s+i) = c;
		c = getchar();
		i++;
		s = (char *)realloc(s,sizeof(char)*(i+1));
	}
	*(s+i) = '\0';
	return s;
}
void error_exit(char *s)
{
	fprintf(stderr, "\nERROR: %s - bye!\n", s);
	exit(1);
}
