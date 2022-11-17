#include <stdio.h>
#include <pthread.h>

#define NUM_STR 10
#define PARENT_NUM 0
#define CHILD_NUM 1

pthread_mutex_t mutex;
pthread_cond_t cond;
int count = 0;

void print(int numStr, int thr){
	pthread_mutex_lock(&mutex);
	while (count % 2 != thr)
		pthread_cond_wait(&cond, &mutex);
	if (!thr)
		printf("parent string %d\n", numStr);
	else
		printf("child string %d\n", numStr);
	count++;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
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
	for (int i = 0; i < 3; i++)
		pthread_mutex_init(&mutex[i], NULL);
	if (st = pthread_create(&thr, NULL, childFunc, NULL)){
		printf("Not create thread: %d\n", st);
		return st;
	}
	for (int c = 0; c < NUM_STR; c++){
		print(c, PARENT_NUM);
	}
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}