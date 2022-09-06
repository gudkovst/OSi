#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

#define NUM_THRS 4
#define NUM_STR 3

void* printingText(void* args){
	char** text = (char**)args;
	for (int i = 0; text[i]; i++)
		printf("%s\n", text[i]);
	return NULL;
}

int main(){
	pthread_t thrs[NUM_THRS];
	int st;
	char** texts[NUM_THRS];
	for (int i = 0; i < NUM_THRS; i++){
		texts[i] = (char**)malloc(NUM_STR * sizeof(char*));
		for (int j = 0; j < NUM_STR; j++){
			
		}
	}
	if (st = pthread_create(&thr, NULL, childFunc, NULL)){
		printf("Not create thread: %d\n", st);
		return st;
	}
	if (st = pthread_join(thr, NULL)){
		printf("Not join thread: %d\n", st);
		return st;
	}
	
	return 0;
}