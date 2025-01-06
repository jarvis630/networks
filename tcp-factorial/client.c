#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    int clientsocket, port;
    struct sockaddr_in serveraddr;
    socklen_t len;
    int num, result = 0;

    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char*)&serveraddr, sizeof(serveraddr));
    len = sizeof(serveraddr);

    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port);

    printf("\nTrying to connect to the server\n");
    connect(clientsocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    printf("\nConnected to the server\n");

    printf("Enter a number to calculate its factorial: ");
    scanf("%d", &num);

    send(clientsocket, &num, sizeof(num), 0);

    printf("\nReceiving factorial from server\n");
    recv(clientsocket, &result, sizeof(result), 0);
    printf("\nFactorial received: %d\n", result);

    close(clientsocket);

    return 0;
}
