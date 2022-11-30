#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libgen.h>

int main(){
	FILE* fptrs[2];
	int n;
	pid_t pid = p2open("sort", fptrs);
	if (pid == -1){
		perror("");
		return 2;
	}
	srand(time(NULL));
	for (int i = 0; i < 100; i++)
		fprintf(fptrs[0], "%02d\n", rand() % 100);
	fclose(fptrs[0]);
	for (int i = 1; i <= 100; i++){
		fscanf(fptrs[1], "%d", &n);
		printf("%d ", n);
		if (!(i % 10))
			printf("\n");
	}
	pclose(fptrs[1]);
	return 0;
}