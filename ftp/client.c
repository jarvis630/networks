#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 1025
#define BUFFER_SIZE 1024

void send_file(int server_sock, const char *filename);
void receive_file(int server_sock, const char *filename);

int main() {
    int server_sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int choice;
    char filename[100];
    FILE *fptr;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("172.16.9.9");

    if (connect(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Connect error");
        close(server_sock);
        exit(1);
    }

    while (1) {
        printf("\n1. Upload a file (PUT)\n2. Download a file (GET)\n3. Exit (BYE)\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename: ");
                scanf("%s", filename);
                send_file(server_sock, filename);
                break;

            case 2:
                printf("Enter filename: ");
                scanf("%s", filename);
                receive_file(server_sock, filename);
                break;

            case 3:
                strcpy(buffer, "BYE");
                send(server_sock, buffer, sizeof(buffer), 0);
                printf("Connection closed\n");
                close(server_sock);
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void send_file(int server_sock, const char *filename) {
    FILE *fptr = fopen(filename, "r");
    char buffer[BUFFER_SIZE];

    if (!fptr) {
        perror("File doesn't exist");
        return;
    }

    strcpy(buffer, "PUT");
    send(server_sock, buffer, sizeof(buffer), 0);

    bzero(buffer, sizeof(buffer));
    strcpy(buffer, filename);
    send(server_sock, buffer, sizeof(buffer), 0);

    while (fgets(buffer, sizeof(buffer), fptr)) {
        send(server_sock, buffer, sizeof(buffer), 0);
        bzero(buffer, sizeof(buffer));
    }

    strcpy(buffer, "END$");
    send(server_sock, buffer, sizeof(buffer), 0);

    fclose(fptr);
    printf("File '%s' sent successfully.\n", filename);
}

void receive_file(int server_sock, const char *filename) {
    FILE *fptr;
    char buffer[BUFFER_SIZE];

    strcpy(buffer, "GET");
    send(server_sock, buffer, sizeof(buffer), 0);

    bzero(buffer, sizeof(buffer));
    recv(server_sock, buffer, sizeof(buffer), 0);
    printf("Server Process ID: %s\n", buffer);

    bzero(buffer, sizeof(buffer));
    strcpy(buffer, filename);
    send(server_sock, buffer, sizeof(buffer), 0);

    bzero(buffer, sizeof(buffer));
    recv(server_sock, buffer, sizeof(buffer), 0);

    if (strcmp(buffer, "404") == 0) {
        printf("File '%s' does not exist on the server.\n", filename);
        return;
    } else if (strcmp(buffer, "200") == 0) {
        fptr = fopen(filename, "w");
        if (!fptr) {
            perror("Error opening file for writing");
            return;
        }

        while (1) {
            bzero(buffer, sizeof(buffer));
            recv(server_sock, buffer, sizeof(buffer), 0);
            if (strcmp(buffer, "END$") == 0)
                break;
            fprintf(fptr, "%s", buffer);
        }

        fclose(fptr);
        printf("File '%s' received successfully.\n", filename);
    }
}
