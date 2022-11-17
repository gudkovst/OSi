#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

#define NUM_THRS 4
#define NUM_STR 3
#define LEN_STR 2
#define CODE 32

void* printingText(void* args){
	char** text = (char**)args;
	for (int i = 0; i < NUM_STR; i++)
		printf("%s\n", text[i]);
	return NULL;
}

int main(){
	pthread_t thrs[NUM_THRS];
	int st;
	char*** texts = (char***)malloc(NUM_THRS * sizeof(char**));
	for (int i = 0; i < NUM_THRS; i++){
		texts[i] = (char**)malloc(NUM_STR * sizeof(char*));
		for (int j = 0; j < NUM_STR; j++){
			texts[i][j] = (char*)malloc(LEN_STR * sizeof(char));
			for (int k = 0; k < LEN_STR; k++){
				texts[i][j][k] = ('a' + k % CODE) * (j % 2) + ('A' + k % CODE) * !(j % 2) + i;
			}
			texts[i][j][LEN_STR] = '\0';
		}
	}
	for (int i = 0; i < NUM_THRS; i++)
		if (st = pthread_create(&thrs[i], NULL, printingText, texts[i])){
			fprintf(stderr, "Not create thread: %d\n", st);
			return st;
		}
	for (int i = 0; i < NUM_THRS; i++)
		if (st = pthread_join(thrs[i], NULL)){
			fprintf(stderr, "Not join thread: %d\n", st);
			return st;
		}
	for (int i = 0; i < NUM_THRS; i++){
		for (int j = 0; j < NUM_STR; j++)
			free(texts[i][j]);
		free(texts[i]);
	}
	free(texts);
	return 0;
}