#include <stdio.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <netdb.h> 

int main() { 
    int serversocket, port; 
    struct sockaddr_in serveraddr, clientaddr; 
    socklen_t len; 
    char message[50]; 

    serversocket = socket(AF_INET, SOCK_DGRAM, 0); 

    bzero((char*)&serveraddr, sizeof(serveraddr)); 
    serveraddr.sin_family = AF_INET; 

    printf("Enter the port number: "); 
    scanf("%d", &port); 
    serveraddr.sin_port = htons(port); 
    serveraddr.sin_addr.s_addr = INADDR_ANY; 

    bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 

    printf("\nWaiting for the client connection\n"); 

    bzero((char*)&clientaddr, sizeof(clientaddr)); 
    len = sizeof(clientaddr); 

    recvfrom(serversocket, message, sizeof(message), 0, (struct sockaddr*)&clientaddr, &len); 

    printf("\nConnection received from client.\n"); 
    printf("\nThe client has sent:\t%s\n", message); 

    printf("\nSending message to the client.\n"); 
    sendto(serversocket, "YOUR MESSAGE RECEIVED.", 
           sizeof("YOUR MESSAGE RECEIVED."), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr)); 

    close(serversocket); 
} 
