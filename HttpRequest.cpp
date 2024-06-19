#include "HttpRequest.h"

std::string HttpRequest::build_get_request(const std::string& hostname, const std::string& port) const {
    std::string request = "GET / HTTP/1.1\r\n";
    request += "Host: " + hostname + ":" + port + "\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";
    return request;
}
