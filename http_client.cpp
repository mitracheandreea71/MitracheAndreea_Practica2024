#include "http_client.h"
#include <iostream>

HTTPClient::HTTPClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error initializing curl." << std::endl;
    }
}

HTTPClient::~HTTPClient() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

bool HTTPClient::downloadFile(const std::string& url, const std::string& outputFileName) {
    FILE* fp = fopen(outputFileName.c_str(), "wb");
    if (!fp) {
        std::cerr << "Failed to open file for writing" << std::endl;
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    struct curl_slist* headers = NULL;
    if (!jwtToken.empty()) {
        std::string bearer = "Authorization: Bearer " + jwtToken;
        headers = curl_slist_append(headers, bearer.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    CURLcode res = curl_easy_perform(curl);
    fclose(fp);
    if (headers) {
        curl_slist_free_all(headers);
    }

    if (res != CURLE_OK) {
        std::cerr << "Failed to download: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}

size_t HTTPClient::writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t realsize = size * nmemb;
    userp->append((char*)contents, realsize);
    return realsize;
}

bool HTTPClient::get(const std::string& url, std::string& response) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::string respBuffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBuffer);

    struct curl_slist* headers = NULL;
    if (!jwtToken.empty()) {
        std::string bearer = "Authorization: Bearer " + jwtToken;
        headers = curl_slist_append(headers, bearer.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    CURLcode res = curl_easy_perform(curl);
    if (headers) {
        curl_slist_free_all(headers);
    }

    if (res != CURLE_OK) {
        std::cerr << "Failed to perform GET request: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    response = respBuffer;
    return true;
}

bool HTTPClient::post(const std::string& url, const std::string& postData, std::string& response) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

    std::string respBuffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBuffer);

    struct curl_slist* headers = NULL;
    if (!jwtToken.empty()) {
        std::string bearer = "Authorization: Bearer " + jwtToken;
        headers = curl_slist_append(headers, bearer.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    CURLcode res = curl_easy_perform(curl);
    if (headers) {
        curl_slist_free_all(headers);
    }

    if (res != CURLE_OK) {
        std::cerr << "Failed to perform POST request: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    response = respBuffer;
    return true;
}

void HTTPClient::setCookie(const std::string& cookie) {
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
}

void HTTPClient::setJWT(const std::string& jwt) {
    jwtToken = jwt;
    std::cout << "JWT set: " << jwtToken << std::endl;
}
