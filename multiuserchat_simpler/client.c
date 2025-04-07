#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 7500

void main() {
	int clientsocket;
	struct sockaddr_in serveraddr;
	socklen_t len;
	char buffer[100];

	memset(&serveraddr, 0, sizeof(serveraddr));
	len = sizeof(serveraddr);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = PORT;
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	clientsocket = socket(AF_INET, SOCK_STREAM, 0);
	connect(clientsocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	printf("\nConnected to server.");

	printf("\nEnter the message: ");
	while(fgets(buffer, sizeof(buffer), stdin) != "exit") {
		send(clientsocket, buffer, sizeof(buffer), 0);
		printf("\nMessage sent.");

		recv(clientsocket, buffer, sizeof(buffer), 0);
		printf("\nReply: %s",buffer);
		printf("\nEnter new message: ");
	}

	close(clientsocket);
}
