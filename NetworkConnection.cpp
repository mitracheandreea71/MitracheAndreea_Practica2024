#include "NetworkConnection.h"
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h> // Adaugă acest header pentru inet_ntop

NetworkConnection::NetworkConnection(const std::string& server_ip, const std::string& server_port) {
    struct addrinfo hints, *server_info;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(server_ip.c_str(), server_port.c_str(), &hints, &server_info);
    if (status != 0) {
        std::cerr << "Error: getaddrinfo: " << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }

    sockfd_ = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (sockfd_ == -1) {
        perror("Error: socket");
        exit(EXIT_FAILURE);
    }

    status = connect(sockfd_, server_info->ai_addr, server_info->ai_addrlen);
    if (status == -1) {
        perror("Error: connect");
        exit(EXIT_FAILURE);
    }

    // Stocare hostname și port pentru utilizare ulterioară
    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)server_info->ai_addr;
    void *addr = &(ipv4->sin_addr);
    inet_ntop(server_info->ai_family, addr, ip, INET_ADDRSTRLEN);
    hostname_ = std::string(ip);
    port_ = server_port;

    freeaddrinfo(server_info);
}

NetworkConnection::~NetworkConnection() {
    close(sockfd_);
}

void NetworkConnection::send_request(const std::string& request) {
    int bytes_sent = send(sockfd_, request.c_str(), request.length(), 0);
    if (bytes_sent == -1) {
        perror("Error: send");
        exit(EXIT_FAILURE);
    }
}

int NetworkConnection::sockfd() const {
    return sockfd_;
}

std::string NetworkConnection::hostname() const {
    return hostname_;
}

std::string NetworkConnection::port() const {
    return port_;
}
