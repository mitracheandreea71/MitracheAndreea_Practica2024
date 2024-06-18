#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define INITIAL_BUFFER_SIZE 8192 // Initial buffer size

void send_http_get(const char *hostname, const char *path) {
    int sockfd;
    struct addrinfo hints, *res, *p;
    char request[1024];
    char *response = malloc(INITIAL_BUFFER_SIZE);
    int buffer_size = INITIAL_BUFFER_SIZE;

    if (response == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Prepare the hints structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Resolve the hostname to an address
    if (getaddrinfo(hostname, "80", &hints, &res) != 0) {
        perror("getaddrinfo failed");
        free(response);
        exit(EXIT_FAILURE);
    }

    // Iterate through the results and connect to the first we can
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) {
            break;
        }

        close(sockfd);
    }

    if (p == NULL) {
        perror("Failed to connect");
        free(response);
        freeaddrinfo(res); // Free addrinfo on failure
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res); // No longer needed

    // Build HTTP GET request
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, hostname);

    // Send request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Send failed");
        free(response);
        close(sockfd); // Close socket on error
        exit(EXIT_FAILURE);
    }

    // Receive response
    int total_bytes = 0;
    int bytes_received;
    while ((bytes_received = recv(sockfd, response + total_bytes, buffer_size - total_bytes, 0)) > 0) {
        total_bytes += bytes_received;
        // Check if we need to increase the buffer size
        if (total_bytes >= buffer_size) {
            buffer_size *= 2;
            char *new_response = realloc(response, buffer_size);
            if (new_response == NULL) {
                perror("Failed to reallocate memory");
                free(response);
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            response = new_response;
        }
    }

    // Handle errors (e.g., bytes_received < 0)
    if (bytes_received < 0) {
        perror("Error receiving response");
        free(response);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Add terminator
    response[total_bytes] = '\0';

    // Find the end of headers
    char *header_end = strstr(response, "\r\n\r\n");
    if (header_end != NULL) {
        // Print only the headers
        *header_end = '\0';
        printf("Response:\n%s\n", response);
    } else {
        printf("Response:\n%s\n", response);
    }

    // Close socket and free memory
    close(sockfd);
    free(response);
}

int main() {
    const char *hostname = "www.google.com";
    const char *path = "/";

    send_http_get(hostname, path);

    return 0;
}
