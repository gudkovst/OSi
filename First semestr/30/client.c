#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SIZE 256

int main(){
	char buf[SIZE];
	int len;
	int client = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client == -1){
		perror("No client\n");
		return 1;
	}
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "./socket");
	if (connect(client, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		perror("No connect");
		return 2;
	}
	while (len = read(0, buf, SIZE))
		send(client, buf, len, 0);
	return 0;
}