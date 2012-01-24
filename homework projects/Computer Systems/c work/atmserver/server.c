/* 
 joe mahon
 */
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

void error(char *msg){
    perror(msg);
    exit(0);
}

void *ConnectionThread(void *arg);

typedef struct packet{
	int signal; //client sends 0, 1, or 2 as type; server sends 0 or 1 for failure or success
	int amt; //client sends amount to update with, server sends new total amount after tx
}packet_t;

pthread_mutex_t ACCOUNT;
int totalMoney;

typedef struct transactionHist{
	int kind;
	int amt;
	struct transactionHist *prev;
}tHist_t;

tHist_t *thisTHist;

int main(int argc, char *argv[])
{
	thisTHist = (tHist_t *)malloc(sizeof(tHist_t));
	thisTHist->prev = NULL;
	thisTHist->kind = 0;
	thisTHist->amt = 150;
	
	totalMoney = 150;
	
	int sock, *newsock, len, fromlen, n, retval;
	unsigned short port;
	struct sockaddr_in server, from;   
	pthread_t tid;

	if (argc < 2) {
		fprintf(stderr,"usage %s portnumber\n",argv[0]);
		exit(0);
	}
	port = (unsigned short) atoi(argv[1]);
	sock=socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) error("Opening socket");
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);  
	len=sizeof(server);
	if (bind(sock, (struct sockaddr *)&server, len) < 0) 
		error("binding socket");
	fromlen=sizeof(from);
	if (listen(sock,5) < 0) 
		error("listening");
	
	pthread_mutex_init(&ACCOUNT,NULL);
	
	while (1) {
		newsock = (int *)malloc(sizeof (int));
		
		*newsock=accept(sock, (struct sockaddr *)&from, &fromlen);
		if (*newsock < 0) error("Accepting");
		printf("A connection has been accepted from %s\n", inet_ntoa((struct in_addr)from.sin_addr));
		retval = pthread_create(&tid, NULL, ConnectionThread, (void *)newsock);
		if (retval != 0){
            error("Error, could not create thread");
		}
	} 

	return 0; // we never get here 		
}

void *ConnectionThread(void *arg)
{
    int sock, n;
	int counter;
	packet_t *readIn = (packet_t *)malloc(sizeof(packet_t));
	
    sock = *(int *)arg;
	
    n = read(sock,readIn,sizeof(packet_t));
	tHist_t *newTx = (tHist_t *)malloc(sizeof(tHist_t));
	newTx->prev = thisTHist;
	newTx->amt = readIn->amt;
	newTx->kind = readIn->signal;
	thisTHist = newTx; //updates the TX history records
	
	switch(readIn->signal){
		case 0:
			//perform a deposit
			pthread_mutex_lock(&ACCOUNT);
			totalMoney += readIn->amt;
			printf("deposited $%d\n", readIn->amt);
			pthread_mutex_unlock(&ACCOUNT);
			break;
		case 1:
			//perform a withdrawl
			pthread_mutex_lock(&ACCOUNT);
			totalMoney -= readIn->amt;
			printf("withdrew $%d\n", readIn->amt);
			pthread_mutex_unlock(&ACCOUNT);
			break;
		case 2:
			//print out the last 10 transactions
			counter = 0;
			tHist_t *ptr = thisTHist;
			printf("LAST 10 TRANSACTIONS, MOST RECENT FIRST\n");
			while(counter < 10){
				printf("Transaction type:	%d, transaction amount:		%d\n", ptr->kind, ptr->amt);
				ptr = ptr->prev;
				counter++;
			}
			break;
	}
	
	packet_t *res = (packet_t *)malloc(sizeof(packet_t));
	res->signal = 1;
	res->amt = totalMoney;
	n = write(sock, res, sizeof(packet_t));
	
	if (close(sock) < 0)
		error("closing");
    pthread_exit(NULL);
    return NULL;
}
