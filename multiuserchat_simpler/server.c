#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 7500

void main() {
	int serversocket, clientsocket;
	struct sockaddr_in serveraddr, clientaddr;
	socklen_t len;
	char buffer[100], clientinfo[50];
	pid_t child;

	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(&clientaddr, 0, sizeof(clientaddr));
	len = sizeof(clientaddr);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = PORT;
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	serversocket = socket(AF_INET, SOCK_STREAM, 0);
	bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	printf("\nWaiting for connections...");
	listen(serversocket, 5);

	while(1) {
		clientsocket = accept(serversocket, (struct sockaddr*)&clientaddr, &len);
		inet_ntop(AF_INET, &(clientaddr.sin_addr), clientinfo, sizeof(clientinfo));

		if((child = fork()) == 0) {
			close(serversocket);
			printf("\nConnected to %s",clientinfo);
			while(1) {
				memset(buffer, 0, sizeof(buffer));
				recv(clientsocket, buffer, sizeof(buffer), 0);

				printf("\nClient: %s. Message: %s",clientinfo, buffer);
				send(clientsocket, "Message recieved.", sizeof("Message recieved."), 0);
				printf("\nReply sent to %s.",clientinfo);
			}
		}

		close(clientsocket);
	}
}
