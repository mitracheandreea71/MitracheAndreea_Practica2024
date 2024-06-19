#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include <string>

class NetworkConnection {
public:
    NetworkConnection(const std::string& server_ip, const std::string& server_port);
    ~NetworkConnection();
    void send_request(const std::string& request);
    int sockfd() const;
    std::string hostname() const;
    std::string port() const;

private:
    int sockfd_;
    std::string hostname_;
    std::string port_;
};

#endif // NETWORKCONNECTION_H
