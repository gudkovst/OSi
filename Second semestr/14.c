#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_STR 10
#define PARENT_NUM 0
#define CHILD_NUM 1

sem_t sem[2];

void print(int numStr, int nthr){
	sem_wait(&sem[nthr]);
	if (!nthr)
		printf("parent string %d\n", numStr);
	else
		printf("child string %d\n", numStr);
	sem_post(&sem[!nthr]);
}

void* childFunc(void* args){
	for (int c = 0; c < NUM_STR; c++){
		print(c, CHILD_NUM);
	}
	return NULL;
}

int main(){
	pthread_t thr;
	int st;
	for (int i = 0; i < 2; i++)
		if (sem_init(&sem[i], 0, !i) == -1){
			perror("");
			return -1;
		}
	if (st = pthread_create(&thr, NULL, childFunc, NULL)){
		fprintf(stderr, "Not create thread: %d\n", st);
		return st;
	}
	for (int c = 0; c < NUM_STR; c++){
		print(c, PARENT_NUM);
	}
	if (st = pthread_join(thr, NULL)){
		fprintf(stderr, "Not join thread: %d\n", st);
		return st;
	}
	for (int i = 0; i < 2; i++)
		if (sem_destroy(&sem[i]) == -1){
			perror("");
			return -1;
		}
	return 0;
}