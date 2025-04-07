#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 7500

void main() {
	int clientsocket;
	struct sockaddr_in serveraddr;
	socklen_t len;
	char buffer[50];

	memset(&serveraddr, 0, sizeof(serveraddr));
	len = sizeof(serveraddr);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = PORT;
	serveraddr.sin_addr.s_addr = inet_addr("10.0.2.4");

	clientsocket = socket(AF_INET, SOCK_DGRAM, 0);

	printf("\nSending time request...");
	memset(buffer, 0, sizeof(buffer));
	sendto(clientsocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	recvfrom(clientsocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serveraddr, &len);
	printf("\nRecieved time: %s",buffer);

	printf("\nClosing connection...\n");
	close(clientsocket);
}
