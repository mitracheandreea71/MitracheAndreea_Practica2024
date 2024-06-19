#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();
    std::string receive_response(int sockfd) const;
};

#endif // HTTPRESPONSE_H
