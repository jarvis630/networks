#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int sockfd, newSockFd, size, firstTime = 1, currentPacket, wait = 3;
    char data[100], digit[2];
    struct sockaddr_in client;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    client.sin_family = AF_INET;
    client.sin_port = 7745;
    client.sin_addr.s_addr = INADDR_ANY;

    printf("\nStarting up...");
    size = sizeof(client);
    printf("\nEstablishing Connection to Server...");
    connect(sockfd, (struct sockaddr*)&client, size);

    sprintf(data, "REQUEST");
    send(sockfd, data, strlen(data), 0);

    do {
        recv(sockfd, data, 100, 0);
        currentPacket = atoi(data);
        printf("\nGot packet: %d", currentPacket);

        if (currentPacket == 3 && firstTime) {
            printf("\n*** Simulation: Packet data corrupted or incomplete.");
            printf("\n*** Sending RETRANSMIT for packet 1.");
            memset(&data, 0, sizeof(data));
            sprintf(data, "R1");
            send(sockfd, data, strlen(data), 0);
            firstTime = 0;
        } else {
            wait--;
        }

        if (!wait) {
            printf("\n*** Packet Accepted -> Sending ACK");
            wait = 3;
            sprintf(data, "A");
            digit[0] = (char)(currentPacket + 48);
            digit[1] = '\0';
            strcat(data, digit);
            send(sockfd, data, strlen(data), 0);
        }
    } while (currentPacket != 9);

    printf("\nAll packets received...Exiting.\n");
    close(sockfd);
    return 0;
}







