#include "bathroom.h"

int bathroomOwner;
int stalls[3];


int main(){
	int i;

	//for random numbers
	int stime;
	long ltime;
	ltime = time(NULL);
	stime = (unsigned) ltime/2;
	srand(ltime);
	
	//initialize the CV and the mutex
	pthread_cond_init(&self, NULL);
	if (pthread_mutex_init(&BATHROOM, NULL)) {
		perror("pthread_mutex_init");
		exit(1);
	}

	//create some person threads
	for(i=0; i<25; i++){
		//create thread on each slot of person pthread array, always NULL, 
				//with initial subroutine of (void *)person, with NULL arguments
		if (pthread_create(&(people[i]), NULL, (void *)person, NULL ) != 0) {
			perror("pthread_create");
			exit(1);
		}
	}
	
	for(i = 0; i<25; i++){
		//wait for all person threads to die, having used the bathroom
		pthread_join(people[i], NULL);
	}
	
	printf("\nOK, everyone's done. Closing process and mopping up now.\n");
	//no need for pthread_kill, since all the threads should have killed themselves
	
	exit(0);
}


void canThreadUseBathroom(int gender){
	pthread_mutex_lock(&BATHROOM);
//	printf("		checking...\n");
	while((bathroomOwner != gender && usedStalls() > 0) || usedStalls() == 3){
	/*	switch(gender){
			case 0:
				printf("	Woman can't enter the bathroom right now\n");
				//woman cannot enter bathroom, as it is owned by men right now or there are already too many girls in there
				break;
			case 1:
				printf("	Man can't enter the bathroom right now\n");
				//man cannot enter bathroom, as it is owned by women right now or there are already too many guys in there
				break;
		}*/
		pthread_cond_wait(&self, &BATHROOM);
	}
	pthread_mutex_unlock(&BATHROOM);
	return;
}

void person(){
	//generate a random gender
	int gender = -1;
	gender = rand()%2;
	//printf("person of gender %d created", gender);
	
	canThreadUseBathroom(gender);
	
	int i = 0;
	for(i=0; i<3; i++){
		if(stalls[i] == 0){
			stalls[i] = 1;
			break;
		}
	}
	if(i == 3)
		printf("	We have a problem with the stalls.\n");
	
	bathroomOwner = gender; //redundant in most cases, except when there had just been 0 people in the bathroom
	printf("Person of gender %d just entered bathroom for....stall %d\n", gender, i+1);
	//printf("		usedStalls: %d\n", usedStalls());
	//printf("Bathroom owned by %d right now\n", bathroomOwner);
	
	int btime = rand()%4+1; //random amount of time in the bathroom
	sleep(btime); //using the bathroom
	stalls[i] = 0;
	printf("Person of gender %d just left the bathroom from..stall %d\n", gender, i+1);

	pthread_cond_broadcast(&self);
	pthread_mutex_unlock(&BATHROOM);
	return;
}



//return the number of stalls taken up in the bathroom at the moment function is called
int usedStalls(){
	int i = 0;
	int result = 0;
	for(i=0; i<3; i++)
		if(stalls[i] == 1)
			result++;
	return result;
}



