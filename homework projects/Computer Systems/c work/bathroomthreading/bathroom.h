#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


void canThreadUseBathroom(int gender);
void person();
int usedStalls();
pthread_mutex_t BATHROOM;
pthread_t people[25]; //generate 25 people of random genders
pthread_cond_t self;
