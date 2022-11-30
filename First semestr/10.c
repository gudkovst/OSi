#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	if (argc < 2)
		return 5;
	pid_t pid = fork();
	if (pid == -1){
		perror("");
		return 1;
	}
	if (!pid){
		execvp(argv[1], argv + 1);
		return 0;
	}
	else{
		int st;
		if (wait(&st) == -1){
			perror("");
			return 2;
		}
		if (WIFEXITED(st))
			printf("child exit with code %d\n", WEXITSTATUS(st));
	}
	return 0;
}