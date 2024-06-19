#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>

class HttpRequest {
public:
    std::string build_get_request(const std::string& hostname, const std::string& port) const;
};

#endif // HTTPREQUEST_H
