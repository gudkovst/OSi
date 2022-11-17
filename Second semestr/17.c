#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <signal.h>
#include <string.h>

#define MAX_LEN_STR 80
#define TIME_SLEEP 5

typedef struct List {
	char* data;
	struct List* prev;
	struct List* next;
} list;

list* head = NULL;
pthread_mutex_t listMutex;

void push(char* data){
	list* newHead = (list*)malloc(sizeof(list));
	newHead->data = data;
	newHead->prev = NULL;
	pthread_mutex_lock(&listMutex);
	newHead->next = head;
	if (head)
		head->prev = newHead;
	head = newHead;
	pthread_mutex_unlock(&listMutex);
}

void swap(list* f, list* s){
	if (head == f)
		head = s;
	else
		f->prev->next = s;
	if (s->next)
		s->next->prev = f;
	f->next = s->next;
	s->next = f;
	s->prev = f->prev;
	f->prev = s;
}

void* sort(void* args){
	while(1){
		sleep(TIME_SLEEP);
		pthread_mutex_lock(&listMutex);
		for (list* i = head; i; i = i->next)
			for (list* j = i; j && j->prev; j = j->prev)
				if (strcmp(j->data, j->prev->data) < 0)
					swap(j->prev, j);
		pthread_mutex_unlock(&listMutex);
	}
}

void printList(){
	pthread_mutex_lock(&listMutex);
	for (list* iter = head; iter; iter = iter->next)
		printf("%s\n", iter->data);
	pthread_mutex_unlock(&listMutex);
}

int endWork(pthread_t thr){
	pthread_mutex_lock(&listMutex);
	pthread_cancel(thr);
	pthread_join(thr, NULL);
	pthread_mutex_unlock(&listMutex);
	printList();
	while (head){
		list* node = head;
		head = head->next;
		free(node->data);
		free(node);
	}
	return pthread_mutex_destroy(&listMutex);
}

int main(){
	int st;
	pthread_t sorter;
	if (st = pthread_mutex_init(&listMutex, NULL)){
		fprintf(stderr, "mutex not initialized: %d\n", st);
		return st;
	}
	if (st = pthread_create(&sorter, NULL, sort, NULL)){
		fprintf(stderr, "Not create thread: %d\n", st);
		return st;
	}
	while(1){
		char* str = (char*)malloc(MAX_LEN_STR * sizeof(char));
		if (!fgets(str, MAX_LEN_STR, stdin))
			break;
		int len = strlen(str);
		if (len == 1)
			printList();
		else {
			str[len - 1] = '\0';
			push(str);
		}
	}
	return endWork(sorter);
}