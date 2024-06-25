#include "http_client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <URL> [<output_file_name>]" << std::endl;
        std::cerr << "       " << argv[0] << " GET <URL>" << std::endl;
        std::cerr << "       " << argv[0] << " POST <URL> '<post_data>'" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    HTTPClient httpClient;

    if (command == "GET" && argc == 3) {
        std::string url = argv[2];
        std::string response;
        if (httpClient.get(url, response)) {
            std::cout << "GET Request Response:\n" << response << std::endl;
        }
    } else if (command == "POST" && argc == 4) {
        std::string url = argv[2];
        std::string postData = argv[3];
        std::string response;
        if (httpClient.post(url, postData, response)) {
            std::cout << "POST Request Response:\n" << response << std::endl;
        }
    } else if (argc >= 3) {
        std::string url = argv[1];
        std::string outputFileName = argv[2];
        if (httpClient.downloadFile(url, outputFileName)) {
            std::cout << "Downloaded file from " << url << " saved as " << outputFileName << std::endl;
        }
    } else {
        std::cerr << "Invalid arguments." << std::endl;
        return 1;
    }

    return 0;
}
