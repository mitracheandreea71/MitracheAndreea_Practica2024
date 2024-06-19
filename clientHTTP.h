#ifndef CLIENTHTTP_H
#define CLIENTHTTP_H

#include <string>
#include <vector>
#include "NetworkConnection.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

class ClientHTTP {
public:
    ClientHTTP(const std::string& server_ip, const std::string& server_port);
    ~ClientHTTP();

    void send_http_get();

private:
    NetworkConnection network_connection_;
    HttpResponse http_response_;
    HttpRequest http_request_;
    std::vector<std::pair<std::string, std::string>> cookies_;

    void extract_cookies(const std::string& response);
    void add_cookies_to_request(std::string& request) const;
    void print_cookies() const;
};

#endif // CLIENTHTTP_H
