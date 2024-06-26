#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <map>
#include <curl/curl.h>

class HTTPClient {
public:
    HTTPClient();
    ~HTTPClient();

    bool downloadFile(const std::string& url, const std::string& outputFileName);
    bool get(const std::string& url, std::string& response);
    bool post(const std::string& url, const std::string& postData, std::string& response);
    void setCookie(const std::string& cookie);
    void setJWT(const std::string& jwt);

private:
    CURL* curl;
    std::string jwtToken;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};

#endif 
