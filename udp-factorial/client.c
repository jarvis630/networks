#include <stdio.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <strings.h> 

int main() { 
    int clientsocket, port, num;
    struct sockaddr_in serveraddr; 
    socklen_t len; 
    char buffer[256]; 

    clientsocket = socket(AF_INET, SOCK_DGRAM, 0); 

    bzero((char*)&serveraddr, sizeof(serveraddr)); 
    len = sizeof(serveraddr); 
    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: "); 
    scanf("%d", &port); 
    serveraddr.sin_port = htons(port); 
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter a number to calculate factorial: ");
    scanf("%d", &num);

    snprintf(buffer, sizeof(buffer), "%d", num);
    sendto(clientsocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    printf("\nWaiting for server response...\n");

    bzero(buffer, 256);
    recvfrom(clientsocket, buffer, 255, 0, (struct sockaddr*)&serveraddr, &len);
    printf("Received from server: %s\n", buffer);

    close(clientsocket);
    return 0;
}
