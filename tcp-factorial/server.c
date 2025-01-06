#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>

int factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

int main() {
    int serversocket, clientsocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    int num, result = 0;

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    bzero((char*)&clientaddr, sizeof(clientaddr));
    len = sizeof(clientaddr);

    listen(serversocket, 5);
    printf("\nWaiting for client connection...\n");

    clientsocket = accept(serversocket, (struct sockaddr*)&clientaddr, &len);
    printf("\nClient connected\n");

    printf("\nReading number from the client\n");
    read(clientsocket, &num, sizeof(num));
    printf("\nThe client has sent the number %d\n", num);

    printf("\nCalculating factorial...\n");
    result = factorial(num);
    printf("Factorial of %d is %d\n", num, result);

    printf("\nSending factorial to the client\n");
    write(clientsocket, &result, sizeof(result));

    close(clientsocket);
    close(serversocket);

    return 0;
}
