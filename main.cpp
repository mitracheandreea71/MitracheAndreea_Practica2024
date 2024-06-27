#include "http_client.h"
#include <iostream>
#include <stdio.h>

int main(int argc, char* argv[]) {
    //std::cout << "Start" << std::endl;
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <URL> [<output_file_name>] [<JWT>] [<Cookie>]" << std::endl;
        std::cerr << "       " << argv[0] << " GET <URL> [<JWT>] [<Cookie>]" << std::endl;
        std::cerr << "       " << argv[0] << " POST <URL> '<post_data>' [<JWT>] [<Cookie>]" << std::endl;
        return 1;
    }
       //std::cout << "Verification step 1" << std::endl;
    std::string command = argv[1];
    std::string jwt = (argc > 3 && command != "GET" && command != "POST") ? argv[3] : "";
    std::string cookie = (argc > 4 && (command == "GET" || command == "POST")) ? argv[4] : "";
       //std::cout << "Verification step 2" << std::endl;
    HTTPClient httpClient;
    if (!jwt.empty()) {
        httpClient.setJWT(jwt);
    }
    if (!cookie.empty()) {
        httpClient.setCookie(cookie);
    }
       //std::cout << "Verification step 3" << std::endl;
    httpClient.setTimeout(30); 
    httpClient.setUserAgent("MyHTTPClient/1.0"); 
    //std::cout<<"verificare";
    //printf("aici\n");
    if (command == "GET" && argc >= 3) {
        std::string url = argv[2];
        std::string response;
        if (httpClient.get(url, response)) {
          std::cout << "GET Request Response:\n" << response << std::endl;
           std::cout << "HTTP Response Code: " << httpClient.getHTTPResponseCode() << std::endl;
           std::cout << "Total Request Time: " << httpClient.getTotalRequestTime() << " seconds" << std::endl;
           std::cout << "Response Size: " << httpClient.getResponseSize() << " bytes" << std::endl;
        }
    } else if (command == "POST" && argc >= 4) {
        std::string url = argv[2];
        std::string postData = argv[3];
        std::string response;
        if (httpClient.post(url, postData, response)) {
           // std::cout <<"post\n";
            std::cout << "POST Request Response:\n" << response << std::endl;
            std::cout << "HTTP Response Code: " << httpClient.getHTTPResponseCode() << std::endl;
            std::cout << "Total Request Time: " << httpClient.getTotalRequestTime() << " seconds" << std::endl;
            std::cout << "Response Size: " << httpClient.getResponseSize() << " bytes" << std::endl;
        }else{
            std::cerr << "Failed to perform POST request." << std::endl;
       
        }
    } else if (argc >= 3) {
        std::string url = argv[1];
        std::string outputFileName = argv[2];
        if (httpClient.downloadFile(url, outputFileName)) {
            std::cout << "Downloaded file from " << url << " saved as " << outputFileName << std::endl;
            std::cout << "HTTP Response Code: " << httpClient.getHTTPResponseCode() << std::endl;
            std::cout << "Total Request Time: " << httpClient.getTotalRequestTime() << " seconds" << std::endl;
            std::cout << "Response Size: " << httpClient.getResponseSize() << " bytes" << std::endl;
        }
    } else {
        std::cerr << "Invalid arguments." << std::endl;
        return 1;
    }

    return 0;
}
