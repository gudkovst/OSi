#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	if (argc < 2)
		return 8;
	pid_t pid = fork();
	if (pid == -1){
		perror("");
		return 1;
	}
	if (!pid){
		execvp("cat", argv);
		perror("");
		return 2;
	}
	else {
		int st;
		if (wait(&st) == -1){
			perror("");
			return 3;
		}
		printf("Parent process text\n");
	}
	return 0;
}