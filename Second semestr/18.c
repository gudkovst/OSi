#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <signal.h>
#include <string.h>

#define MAX_LEN_STR 80
#define TIME_SLEEP 5
#define NUM_SORTER 2

typedef struct List{
	char* data;
	struct List* next;
	pthread_mutex_t mutex;
} list;

list* head = NULL;
pthread_mutex_t headMutex;

void push(char* data){
	list* newHead = (list*)malloc(sizeof(list));
	newHead->data = data;
	pthread_mutex_init(&newHead->mutex, NULL);
	pthread_mutex_lock(&headMutex);
	newHead->next = head;
	head = newHead;
	pthread_mutex_unlock(&headMutex);
}

void swap(list* prev, list* f, list* s){
	if (prev == f){ //f == head
		pthread_mutex_lock(&f->mutex);
		pthread_mutex_lock(&s->mutex);
		f->next = s->next;
		s->next = f;
		pthread_mutex_lock(&headMutex);
		head = s;
		pthread_mutex_unlock(&headMutex);
		pthread_mutex_unlock(&f->mutex);
		pthread_mutex_unlock(&s->mutex);
		return;
	} 
	pthread_mutex_lock(&prev->mutex);
	pthread_mutex_lock(&f->mutex);
	pthread_mutex_lock(&s->mutex);
	prev->next = s;
	pthread_mutex_unlock(&prev->mutex);
	f->next = s->next;
	s->next = f;
	pthread_mutex_unlock(&s->mutex);
	pthread_mutex_unlock(&f->mutex);
}

void* sort(void* args){
	while(1){
		sleep(TIME_SLEEP);
		if (!head)
			continue;
		for (list* i = head->next; i; i = i->next){
			list* prev = head;
			for (list* j = head; j != i && j && j->next; prev = j, j = j->next){
				if (strcmp(j->data, j->next->data) > 0){
					swap(prev, j, j->next);
				}
			}
		}
	}
}

void printList(){
	list* iter = head;
	while (iter){
		pthread_mutex_lock(&iter->mutex);
		printf("%s\n", iter->data);
		list* nextIter = iter->next;
		pthread_mutex_unlock(&iter->mutex);
		iter = nextIter;
	}
}

int endWork(pthread_t thrs[]){
	for (int i = 0; i < NUM_SORTER; i++){
		pthread_cancel(thrs[i]);
		pthread_join(thrs[i], NULL);
	}
	while(head){
		list* node = head;
		head = head->next;
		pthread_mutex_destroy(&node->mutex);
		free(node->data);
		free(node);
	}
	pthread_mutex_destroy(&headMutex);
	return 0;
}

int main(){
	int st;
	pthread_t sorter[NUM_SORTER];
	for (int i = 0; i < NUM_SORTER; i++){
		if (st = pthread_create(&sorter[i], NULL, sort, NULL)){
			fprintf(stderr, "Not create thread: %d\n", st);
			return st;
		}
	}
	while(1){
		char* str = (char*)malloc(MAX_LEN_STR * sizeof(char));
		if (!fgets(str, MAX_LEN_STR, stdin)){
			free(str);
			break;
		}
		int len = strlen(str);
		if (len == 1){
			free(str);
			printList();
		}
		else {
			if (str[len - 1])
				str[len - 1] = '\0';
			push(str);
		}
	}
	return endWork(sorter);
}