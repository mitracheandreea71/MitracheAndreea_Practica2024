#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_RESPONSE_SIZE 4096

// Funcție pentru a trimite o cerere GET către server
void send_http_get(const char *hostname, const char *path) {
    int sockfd;
    struct sockaddr_in server_addr;
    char request[1000], response[MAX_RESPONSE_SIZE];
    
    // Creare socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    
    // Setare adresa serverului
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // Portul HTTP
    if (inet_pton(AF_INET, hostname, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    
    // Conectare la server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    // Construirea cererii HTTP GET
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, hostname);
    
    // Trimiterea cererii HTTP
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    
    // Primirea răspunsului HTTP
    int total_bytes = 0;
    int bytes_received;
    while ((bytes_received = recv(sockfd, response + total_bytes, MAX_RESPONSE_SIZE - total_bytes, 0)) > 0) {
        total_bytes += bytes_received;
    }
    
    // Adăugare terminator pentru șirul de caractere
    response[total_bytes] = '\0';
    
    // Afișarea răspunsului primit
    printf("Response:\n%s\n", response);
    
    // Închiderea socket-ului
    close(sockfd);
}

int main() {
    const char *hostname = "example.com"; // Schimbă cu hostname-ul dorit
    const char *path = "/"; // Schimbă cu calea dorită
    
    send_http_get(hostname, path);
    
    return 0;
}
