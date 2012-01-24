/*
Joe Mahon
*/

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define PACKET 1024

typedef struct packet{
	int signal; //client sends 0, 1, or 2 as type; server sends 0 or 1 for failure or success
	int amt; //client sends amount to update with, server sends new total amount after tx
}packet_t;

void transact(int sock);

void error(char *msg){
    perror(msg);
    exit(0);
}

pthread_t txs[5]; //there will be 5 transactions per client

int main(int argc, char *argv[]){
	int i;
	
	//for random numbers
	int stime;
	long ltime;
	ltime = time(NULL);
	stime = (unsigned) ltime/2;
	srand(ltime);
	
	//server vars
	int *sock = (int *)malloc(sizeof(int));
	unsigned short port;
	struct sockaddr_in server;
	struct hostent *hp;
	
	if (argc != 3) { 
		printf("Usage: %s server port\n", argv[0]);
		exit(1);
	}
	
	//create 5 transactions in different threads
	for(i = 0; i<5; i++){
		
		//build server connection
		*sock = socket(AF_INET, SOCK_STREAM, 0);
		if (*sock < 0) 
			error("Opening socket");
		server.sin_family = AF_INET;
		hp = gethostbyname(argv[1]);
		if (hp==NULL) 
			error("Unknown host");
		memcpy((char *)&server.sin_addr,(char *)hp->h_addr,hp->h_length);
		port = (unsigned short)atoi(argv[2]);
		server.sin_port = htons(port);
		if (connect(*sock, (struct sockaddr *)&server, sizeof server) < 0)
			error("Connecting");
				
		if(pthread_create(&(txs[i]), NULL, (void *)transact, sock) != 0){
			perror("pthread_create");
			exit(1);
		}
	}
	
	for(i = 0; i<5; i++){
		pthread_join(txs[i], NULL);
	}
	free(sock);
	
	printf("This client is all done\n");
	exit(0);
}

void transact(int sock){
	int n, amt;
	packet_t *p = (packet_t *)malloc(sizeof(packet_t));
	int kind = rand()%3;
	switch(kind){
		case 0:
			//create deposit packet with rand amount
			amt = rand()%150+50;
			printf("sending deposit of		%d\n", amt);
			p->signal = 0;
			p->amt = amt;
			break;
		case 1:
			//create withdrawl packet with rand amount
			amt = rand()%100+50; //money should gradually increase as there are more txs
			printf("requesting withdrawl of	%d\n", amt);
			p->signal = 1;
			p->amt = amt;			
			break;
		case 2:
			//create lastTen packet with 00 amt
			printf("requesting last ten transactions\n");
			p->signal = 2;
			p->amt = 0;
			break;
		//end of switch
	}
	
	//send packet
	n = write(sock, p, sizeof(packet_t));

	//wait for a success signal of 1 with the new total amount
	packet_t *res = (packet_t *)malloc(sizeof(packet_t));
	n = read(sock, res, sizeof(packet_t));

	//can print the total here, will print after every tx
	printf("	Current total in server: %d\n",res->amt);

	free(p);
	free(res);
	
	printf("Client thread terminating.\n\n");
	if (close(sock) < 0)
		error("sock closing");
	
	//end process and allow join to occur
	return;
}

