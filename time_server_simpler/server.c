#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 7500

void main() {
	int serversocket;
	struct sockaddr_in serveraddr, clientaddr;
	socklen_t len;
	time_t ct;
	char buffer[50];

	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(&clientaddr, 0, sizeof(clientaddr));
	len = sizeof(clientaddr);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = PORT;
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	serversocket = socket(AF_INET, SOCK_DGRAM, 0);
	bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	while(1) {
		recvfrom(serversocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len);
		printf("\nRequest recieved. Sending time to client...");

		ct = time(NULL);
		strcpy(buffer, ctime(&ct));

		sendto(serversocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
		printf("\nTime sent.");
	}
}
