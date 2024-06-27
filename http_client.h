#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <vector>
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

    void setTimeout(long timeoutSeconds);
    void setUserAgent(const std::string& userAgent);
    void addHeader(const std::string& header);
    void removeHeader(const std::string& header);
    std::vector<std::string> getAllHeaders() const;
    void setSSLVerification(bool verify);
    long getHTTPResponseCode() const;
    double getTotalRequestTime() const;
    size_t getResponseSize() const;

private:
    CURL* curl;
    std::string jwtToken;
    std::vector<std::string> headers;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};

#endif 
