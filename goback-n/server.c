#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void itoa(int number, char numberString[]) {
    numberString[0] = (char)(number + 48);
    numberString[1] = '\0';
}

int main() {
    int sockfd, newSockFd, size, windowStart = 1, windowCurrent = 1, windowEnd = 4, oldWindowStart, flag;
    char buffer[100];
    socklen_t len;
    struct sockaddr_in server, client;
    
    server.sin_family = AF_INET;
    server.sin_port = 7745;
    server.sin_addr.s_addr = INADDR_ANY;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("\nStarting up...");
    int k = bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    
    if (k == -1) {
        printf("\nError in binding.");
    }

    len = sizeof(client);
    listen(sockfd, 1);
    newSockFd = accept(sockfd, (struct sockaddr*)&client, &len);
    recv(newSockFd, buffer, 100, 0);
    fcntl(newSockFd, F_SETFL, O_NONBLOCK);

    printf("\nReceived a request from client. Sending packets one by one.");

    do {
        if (windowCurrent != windowEnd) {
            itoa(windowCurrent, buffer);
            send(newSockFd, buffer, 100, 0);
            printf("\nPacket Sent: %d\n", windowCurrent);
            windowCurrent++;
        }

        recv(newSockFd, buffer, 100, 0);

        if (buffer[0] == 'R') {
            printf("\n* Received a RETRANSMIT packet.\n Resending packet no. %c...", buffer[1]);
            itoa(atoi(&buffer[1]), buffer);
            send(newSockFd, buffer, 100, 0);
            windowCurrent = atoi(&buffer[0]);
            windowCurrent++;
        }
        else if (buffer[0] == 'A') {
            oldWindowStart = windowStart;
            windowStart = atoi(&buffer[1]) + 1;
            windowEnd += (windowStart - oldWindowStart);
            printf("\n* Received ACK %c. Moving window boundary.", buffer[1]);
        }
    } while (windowCurrent != 10);

    close(sockfd);
    close(newSockFd);
    printf("\nSending Complete. Sockets closed. Exiting...\n");

    return 0;
}



