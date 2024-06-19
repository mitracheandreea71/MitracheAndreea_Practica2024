#include "clientHTTP.h"

int main() {
    std::string server_ip = "127.0.0.1"; // Adresa IP a serverului
    std::string server_port = "8080";    // Portul serverului

    ClientHTTP client(server_ip, server_port);
    client.send_http_get();

    return 0;
}
