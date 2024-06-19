#include "HttpResponse.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

HttpResponse::HttpResponse() {}

HttpResponse::~HttpResponse() {}

std::string HttpResponse::receive_response(int sockfd) const {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    std::string response;
    bool headers_end = false;

    while (!headers_end) {
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == -1) {
            perror("Error receiving response");
            exit(EXIT_FAILURE);
        }
        if (bytes_received == 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        response += buffer;

        // Check if headers are complete (look for double CRLF)
        if (response.find("\r\n\r\n") != std::string::npos) {
            headers_end = true;
            // Remove the content after the headers
            response = response.substr(0, response.find("\r\n\r\n") + 4);
        }
    }

    return response;
}
