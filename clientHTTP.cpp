#include "clientHTTP.h"
#include <iostream>
#include<cstring>

ClientHTTP::ClientHTTP(const std::string& server_ip, const std::string& server_port)
    : network_connection_(server_ip, server_port) {}

ClientHTTP::~ClientHTTP() {}

void ClientHTTP::send_http_get() {
    std::string request = http_request_.build_get_request(network_connection_.hostname(), network_connection_.port());
    add_cookies_to_request(request);  // Adaugă cookie-urile în cererea HTTP GET
    network_connection_.send_request(request);

    std::string response = http_response_.receive_response(network_connection_.sockfd());

    // Extragerea header-elor HTTP din răspuns
    size_t pos = response.find("\r\n\r\n");
    if (pos != std::string::npos) {
        std::string headers = response.substr(0, pos + 4);
        std::cout << "Response:\n" << headers << std::endl;
    } else {
        std::cout << "Response:\n" << response << std::endl;
    }

    extract_cookies(response); 
    print_cookies();
}

void ClientHTTP::add_cookies_to_request(std::string& request) const {
    if (!cookies_.empty()) {
        request += "Cookie: ";
        for (const auto& cookie : cookies_) {
            request += cookie.first + "=" + cookie.second + "; ";
        }
        request.pop_back(); // Remove the last ';'
        request.pop_back(); // Remove the extra space
        request += "\r\n";
    }
}

void ClientHTTP::extract_cookies(const std::string& response) {
    size_t pos = response.find("Set-Cookie:");
    while (pos != std::string::npos) {
        pos += strlen("Set-Cookie:");
        size_t end = response.find("\r\n", pos);
        if (end != std::string::npos) {
            std::string cookie_str = response.substr(pos, end - pos);
            size_t eq_pos = cookie_str.find('=');
            if (eq_pos != std::string::npos) {
                std::string cookie_name = cookie_str.substr(0, eq_pos);
                std::string cookie_value = cookie_str.substr(eq_pos + 1);
                cookies_.push_back({cookie_name, cookie_value});
            }
        }
        pos = response.find("Set-Cookie:", pos + 1);
    }
}

void ClientHTTP::print_cookies() const {
    if (cookies_.empty()) {
        std::cout << "No cookies stored.\n";
        return;
    }

    std::cout << "Stored cookies:\n";
    for (const auto& cookie : cookies_) {
        std::cout << cookie.first << ": " << cookie.second << "\n";
    }
}
