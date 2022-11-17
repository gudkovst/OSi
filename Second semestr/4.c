#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* printing(void* args){
	while (1)
		write(0, "child text\n", 11);
	return NULL;
}

int main(){
	pthread_t thr;
	int st;
	if (st = pthread_create(&thr, NULL, printing, NULL)){
		fprintf(stderr, "Not create thread: %d\n", st);
		return st;
	}
	sleep(2);
	if (st = pthread_cancel(thr)){
		fprintf(stderr, "Not cancel thread: %d\n", st);
		return st;
	}
	if (st = pthread_join(thr, NULL)){
		fprintf(stderr, "Not join thread: %d\n", st);
		return st;
	}
	return 0;
}