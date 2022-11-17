#include <stdio.h>
#include <pthread.h>

#define NUM_STR 10

pthread_mutex_t mutex[3];

void* childFunc(void* args){
	pthread_mutex_lock(&mutex[2]);
	for (int c = 0; c < NUM_STR; c++){
		pthread_mutex_lock(&mutex[c % 3]);
		printf("child string %d\n", c);
		pthread_mutex_unlock(&mutex[(c + 2) % 3]);
	}
	pthread_mutex_unlock(&mutex[(NUM_STR - 1) % 3]);
	return NULL;
}

int main(){
	pthread_t thr;
	int st;
	for (int i = 0; i < 3; i++)
		pthread_mutex_init(&mutex[i], NULL);
	pthread_mutex_lock(&mutex[0]);
	pthread_mutex_lock(&mutex[1]);
	if (st = pthread_create(&thr, NULL, childFunc, NULL)){
		printf("Not create thread: %d\n", st);
		return st;
	}
	for (int c = 0; c < NUM_STR; c++){
		printf("parent string %d\n", c);
		pthread_mutex_unlock(&mutex[c % 3]);
		pthread_mutex_lock(&mutex[(c + 2) % 3]);
	}
	pthread_mutex_unlock(&mutex[NUM_STR % 3]);
	pthread_mutex_unlock(&mutex[(NUM_STR + 1) % 3]);
	for (int i = 0; i < 3; i++)
		pthread_mutex_destroy(&mutex[i]);
	return 0;
}