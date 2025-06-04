#include <iostream>
#include <winsock2.h>

#define IPv4 AF_INET
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

using namespace std;

const int PORT = 5555;
const char* SERVER_IP = "127.0.0.1";

char* receive_message(SOCKET client_socket) {
    static char buffer[1024] = {};
    int received_bytes = recv(client_socket, buffer, sizeof(buffer), 0);

    if (received_bytes <= 0) {
        std::cerr << "Error while connecting to the server!" << std::endl;
        return nullptr;
    }

    return buffer;
}

void send_message(const char* msg, SOCKET client_socket) {
    send(client_socket, msg, strlen(msg), 0);
}



int main() {
    // initialisation for WinSock
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    SOCKET client_socket;
    client_socket = socket(IPv4, TCP, 0);

    // configuration for server address
    sockaddr_in server_addr {};
    server_addr.sin_family = IPv4;                          // address type: IPv4
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);     // interface: localhost
    server_addr.sin_port = htons(PORT);             // port: 5555

    // try to connect with server
    if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == -1) {
        cout << "You cannot connect with server." << endl;
        return -1;
    }
    cout << "Connected with server!" << endl;

    // send and recive message
    send_message("Hello", client_socket);
    cout << "Sended: \t" << "Hello" << endl;

    cout << "Recived:\t" << receive_message(client_socket) << endl;

    closesocket(client_socket);
    WSACleanup();

    return 0;
}