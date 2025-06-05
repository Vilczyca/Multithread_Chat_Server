#include <iostream>
#include <winsock2.h>
#include <thread>

#define IPv4 AF_INET
#define TCP SOCK_STREAM

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const char* SERVER_IP = "127.0.0.1";
const int PORT = 5555;

// Recive message
void receive_messages(SOCKET socket) {
    char buffer[1024];
    int bytes_received;

    while ((bytes_received = recv(socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        cout << buffer << endl;
    }
}


int main() {
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    SOCKET client_socket = socket(IPv4, TCP, 0);

    sockaddr_in server_addr {};
    server_addr.sin_family = IPv4;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) != 0) {
        cerr << "Connection error." << endl;
        return 1;
    }

    cout << "[Connected with server!]" << endl;
    cout << "Choose your username: ";


    //recive messages
    thread recv_thread(receive_messages, client_socket);
    recv_thread.detach();

    // send messages
    string msg;
    while (getline(cin, msg)) {
        send(client_socket, msg.c_str(), msg.length(), 0);
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
