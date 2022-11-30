#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <ctype.h>
#include <sys/uio.h>
#include <unistd.h>

#define SIZE 256

int main(){
	int len, client;
	int server = socket(AF_UNIX, SOCK_STREAM, 0);
	char buf[SIZE];
	if (server == -1){
		printf("Server not created\n");
		return 2;
	}
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "./socket");
	unlink(addr.sun_path);
	if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		perror("bind error");
		return 3;
	}
	if (listen(server, 1) == -1){
		perror("listen error");
		return 4;
	}
	client = accept(server, NULL, NULL);
	if (client == -1){
		perror("accept error");
		return 6;
	}
	while ((len = recv(client, buf, SIZE, 0)) > 0)
		for (int i = 0; i < len; i++)
			printf("%c", toupper(buf[i]));
	if (len < 0){
		perror("recv error");
		return 5;
	}
	close(client);
	unlink("./socket");
	return 0;
}