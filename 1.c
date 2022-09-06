#include <stdio.h>
#include <pthread.h>

void* childFunc(void* args){
	for (char c = 0; c < 10; c++)
		printf("child string %d\n", c);
	return NULL;
}

int main(){
	pthread_t thr;
	int st;
	if (st = pthread_create(&thr, NULL, childFunc, NULL)){
		printf("Not create thread: %d\n", st);
		return st;
	}
	for (char c = 0; c < 10; c++)
		printf("parent string %d\n", c);
	pthread_exit(NULL);
	return 0;
}