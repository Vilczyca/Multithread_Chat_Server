#include <iostream>
#include <winsock2.h>

#define IPv4 AF_INET
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

using namespace std;

const int PORT = 5555;
const char* SERVER_IP = "127.0.0.1";

int main() {
    // initialisation for WinSock
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    SOCKET server_socket;
    server_socket = socket(IPv4, TCP, 0);

    // configuration for server address
    sockaddr_in server_addr {};
    server_addr.sin_family = IPv4;              // address type: IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;   // interface: any (localhost, LAN, etc)
    server_addr.sin_port = htons(PORT); // port: 5555

    bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));

    // listening
    listen(server_socket, 5);
    cout << "Listening at port 5555...\n";


    // accepting connection from client
    SOCKET client_socket;
    sockaddr_in client_addr {};
    int client_size = sizeof(client_addr);

    client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &client_size);
    std::cout << "Connected with client!\n";

    // get data from client
    char buffer[1024] = {};
    recv(client_socket, buffer, sizeof(buffer), 0);
    std::cout << "Recived: " << buffer << "\n";

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}