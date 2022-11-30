#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){
	FILE* f;
	uid_t uid = getuid();
	if (argc < 2){
		fprintf(stderr, "No file\n");
		return 1;
	}
	printf("initially uid = %ld and euid = %ld\n", uid, geteuid());
	f = fopen(argv[1], "r");
	if (!f){
		perror(argv[1]);
		return 2;
	}
	printf("first open accepted\n");
	fclose(f);
	setuid(uid);
	printf("after setuid(%ld):\nuid = %ld and euid = %ld\n", uid, getuid(), geteuid());
	f = fopen(argv[1], "r");
	if (!f){
		perror(argv[1]);
		return 2;
	}
	printf("second open accepted\n");
	fclose(f);
	return 0;
} 