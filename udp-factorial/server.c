#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

int factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

int main() {
    int serversocket, port, num, result;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char buffer[256];

    serversocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serversocket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Error binding");
        exit(1);
    }

    printf("\nWaiting for client message...\n");
    len = sizeof(clientaddr);

    while (1) {
        bzero(buffer, 256);
        int n = recvfrom(serversocket, buffer, 255, 0, (struct sockaddr*)&clientaddr, &len);
        if (n < 0) {
            perror("Error receiving data");
            continue;
        }
        sscanf(buffer, "%d", &num);
        printf("\nReceived number from client: %d\n", num);

        result = factorial(num);
        printf("Factorial of %d is %d\n", num, result);

        snprintf(buffer, sizeof(buffer), "Factorial of %d is %d", num, result);
        n = sendto(serversocket, buffer, strlen(buffer), 0, (struct sockaddr*)&clientaddr, len);
        if (n < 0) {
            perror("Error sending data");
        }
    }

    close(serversocket);
    return 0;
}
