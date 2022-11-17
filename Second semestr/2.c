#include <stdio.h>
#include <pthread.h>

#deine NUM_STR 10

void* childFunc(void* args){
	for (char c = 0; c < NUM_STR; c++)
		printf("child string %d\n", c);
	return NULL;
}

int main(){
	pthread_t thr;
	int st;
	if (st = pthread_create(&thr, NULL, childFunc, NULL)){
		fprintf(stderr, "Not create thread: %d\n", st);
		return st;
	}
	if (st = pthread_join(thr, NULL)){
		fprintf(stderr, "Not join thread: %d\n", st);
		return st;
	}
	for (char c = 0; c < NUM_STR; c++)
		printf("parent string %d\n", c);
	return 0;
}