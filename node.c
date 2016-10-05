#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define SERVER_PORT 4949

int main()
{
	//address
	struct sockaddr_in sin;
	//socket
	int sock_listen;
	//address
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	sin.sin_addr.s_addr = inet_addr("167.205.3.165");
	if((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Error Create Socket\n");
		exit(errno);
	}

	if(bind(sock_listen, (struct sockaddr*) &sin, sizeof(sin))<0){
		printf("error binding");
		exit(errno);
	}
	
	listen(sock_listen, 3);
	
	int client_accept;
	client_accept = accept(sock_listen, NULL, NULL);

	while(1){
		char buf[256];
		recv(client_accept, buf, sizeof(buf),0);
		printf("%s",buf);
	}

	close(sock_listen);

	return 0;
}