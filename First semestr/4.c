#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define N 10

char buf[N];

typedef struct str_list {
	char* data;
	struct str_list* next;
} strList;

int main(){
	strList* head = NULL;
	strList* last, *t;
	int len;
	for (char flag = '1'; flag != '.'; flag = buf[0]){
		fgets(buf, N, stdin);
		len = strlen(buf);
		printf("%d\n", len);
		t = (strList*)malloc(sizeof(strList));
		t->next = NULL;
		t->data = (char*)malloc((len + 1) * sizeof(char));
		for (int i = 0; i < len; i++)
			t->data[i] = buf[i];
		if (!head){
			head = t;
			last = head;
		}
		else {
			last->next = t;
			last = t;
		}
	}
	last = head;
	while (last){
		head = last;
		printf("%s", head->data);
		last = head->next;
		free(head->data);
		free(head);
	}
	return 0;
}