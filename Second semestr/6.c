#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#define koef 2
#define MAX_NUM_THRS 100
#define MAX_LEN_STR 100

void* printing(void* args){
	char* str = (char*)args;
	sleep(koef * strlen(str));
	write(0, str, strlen(str));
	return NULL;
}

int main(){
	pthread_t thrs[MAX_NUM_THRS];
	char* strings[MAX_NUM_THRS];
	int st, count = 0;
	for (; count < MAX_NUM_THRS; count++){
		strings[count] = (char*)malloc(MAX_LEN_STR * sizeof(char));
		if (!fgets(strings[count], MAX_LEN_STR, stdin)){
			free(strings[count]);
			break;
		}
		strings[count][strlen(strings[count]) - 1] = "\0";
	}
	for (int i = 0; i < count; i++)
		if (st = pthread_create(&thrs[i], NULL, printing, (void*)strings[i])){
			fprintf(stderr, "Not create thread %d: %d\n", i, st);
			return st;
		}
	
	for (int i = 0; i < count; i++){
		if (st = pthread_join(thrs[i], NULL)){
			fprintf(stderr, "Not join thread %d: %d\n", i, st);
			return st;
		}
		free(strings[i]);
	}
	return 0;
}