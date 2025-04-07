#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>


#define PORT 7500

typedef struct {
	int frame_id;
	int ack;
	char data[100];
}Frame;

void main() {
	int serversocket;
	struct sockaddr_in serveraddr, clientaddr;
	socklen_t len;
	char buffer[100];
	Frame fr_send, fr_recv;

	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(&clientaddr, 0, sizeof(clientaddr));
	len = sizeof(clientaddr);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = PORT;
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	serversocket = socket(AF_INET, SOCK_DGRAM, 0);
	bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	int expected_frame = 1;
	printf("\nServer started.");

	while(1) {
		recvfrom(serversocket, &fr_recv, sizeof(Frame), 0, (struct sockaddr*)&clientaddr, &len);

		if(fr_recv.frame_id == expected_frame) {
			printf("\nRecieved Frame %d.\nData: %s",fr_recv.frame_id, fr_recv.data);

			fr_send.frame_id = expected_frame;
			fr_send.ack = 1;
			sendto(serversocket, &fr_send, sizeof(Frame), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
			printf("\nAcknowledgement sent.\n");

			expected_frame++;
		}
		else {
			printf("\nOut of order frame recieved.");
		}
	}
}
