#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

int main() {
    int clientsocket, port, num;
    struct sockaddr_in serveraddr;
    socklen_t len;
    char buffer[256];

    clientsocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientsocket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char*)&serveraddr, sizeof(serveraddr));
    printf("Enter the server's IP address: ");
    char server_ip[50];
    scanf("%s", server_ip);
    printf("Enter the port number: ");
    scanf("%d", &port);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(server_ip);

    printf("Enter a number to calculate factorial: ");
    scanf("%d", &num);

    snprintf(buffer, sizeof(buffer), "%d", num);
    sendto(clientsocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    len = sizeof(serveraddr);
    bzero(buffer, 256);
    recvfrom(clientsocket, buffer, 255, 0, (struct sockaddr*)&serveraddr, &len);
    printf("Received from server: %s\n", buffer);

    close(clientsocket);
    return 0;
}
