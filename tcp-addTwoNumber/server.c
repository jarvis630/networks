#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>

int main() {
    int serversocket, clientsocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    int a, b, sum = 0;
    struct serveraddr; 
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
    printf("\nhai");

    clientsocket = accept(serversocket, (struct sockaddr*)&clientaddr, &len);
    printf("\nClient connectivity received\n");

    printf("\nReading numbers from the client\n");
    read(clientsocket, &a, sizeof(a));
    read(clientsocket, &b, sizeof(b));
    printf("\nThe client has sent %d and %d\n", a, b);

    printf("\nSending sum to the client\n");
    sum = a + b;
    printf("sum is %d\n", sum);
    write(clientsocket, &sum, sizeof(sum));

    close(clientsocket);
    close(serversocket);

    return 0;
}
