#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 7500

typedef struct {
	int frame_id;
	int ack;
	char data[100];
}Frame;

void main() {
	int clientsocket;
	struct sockaddr_in serveraddr;
	socklen_t len;
	Frame fr_send, fr_recv;
	char buffer[100];

	memset(&serveraddr, 0, sizeof(serveraddr));
	len = sizeof(serveraddr);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = PORT;
	serveraddr.sin_addr.s_addr = inet_addr("10.0.2.4");

	clientsocket = socket(AF_INET, SOCK_DGRAM, 0);

	int current_frame = 1;
	while(1) {
		printf("\nEnter the data: ");
		scanf("%s",buffer);

		fr_send.frame_id = current_frame;
		strcpy(fr_send.data, buffer);
		fr_send.ack = 0;

		sendto(clientsocket, &fr_send, sizeof(Frame), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		printf("\nFrame %d sent.",current_frame);


		recvfrom(clientsocket, &fr_recv,sizeof(Frame), 0, (struct sockaddr*)&serveraddr, &len);
		if(fr_recv.ack == 1) {
			printf("\nAcknowledgement recieved for frame %d.\n",fr_recv.frame_id);
			current_frame++;
		}
		else {
			printf("\nError");
		}
	}
}
