//#include <iostream>
//#include <winsock2.h>
//#include <thread>
//#include <string>
//#include <mutex>
//#include <vector>
//#include <algorithm>
//
//#define IPv4 AF_INET
//#define TCP SOCK_STREAM
//
//#pragma comment(lib, "ws2_32.lib")
//
//using namespace std;
//
//const int PORT = 5555;
//
//struct Client {
//    string username = "Anonymous";
//    SOCKET socket;
//};
//
//
//vector<Client> clients;
//mutex clients_mutex;
//
//
//// Check if username is already taken
//bool checkUsername(const string& username) {
//    for (const auto& client : clients) {
//        if (client.username == username) {
//            return false;
//        }
//    }
//    return true;
//}
//
//
//// Broadcast to all clients except sender
//void sendToAll(const string& msg, SOCKET sender_socket) {
//    lock_guard<mutex> lock(clients_mutex);
//    for (const auto& client : clients) {
//        if (client.socket != sender_socket) {
//            send(client.socket, msg.c_str(), msg.length(), 0);
//        }
//    }
//}
//
//// Handle individual client
//void handleClient(SOCKET client_socket) {
//    char buffer[1024] = {};
//    string username = "Anonymous";
//
//    // receive username
//    int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
//    if (received <= 0) {
//        closesocket(client_socket);
//        return;
//    }
//    buffer[received] = '\0';
//    username = buffer;
//    {
//        lock_guard<mutex> lock(clients_mutex);
//        if (!checkUsername(username)) {
//            string msg = ">>> Username already taken.\n";
//            send(client_socket, msg.c_str(), msg.size(), 0);
//            closesocket(client_socket);
//            return;
//        }
//
//        clients.push_back(Client{username, client_socket});
//    }
//    cout << ">>> " << username << " joined the chat.\n";
//    sendToAll(">>> " + username + " joined the chat.\n", client_socket);
//
//    while (true) {
//        // recive message
//        memset(buffer, 0, sizeof(buffer));
//        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
//
//        if (bytes_received <= 0) break;
//
//        // response
//        buffer[bytes_received] = '\0';
//        string msg = "[" + username + "]: " + buffer;
//        cout << msg << endl;
//        sendToAll(msg, client_socket);
//    }
//
//    // client disconnected
//    {
//        lock_guard<mutex> lock(clients_mutex);
//        clients.erase(remove_if(clients.begin(), clients.end(),
//                                [&](const Client& c) { return c.socket == client_socket; }),
//                      clients.end());
//    }
//
//    cout << "<<< " << username << " disconnected.\n";
//    sendToAll("<<< " + username + " disconnected.\n", client_socket);
//    closesocket(client_socket);
//}
//
//int main() {
//    WSADATA wsa_data;
//    WSAStartup(MAKEWORD(2, 2), &wsa_data);
//
//    SOCKET server_socket = socket(IPv4, TCP, 0);
//
//    sockaddr_in server_addr {};
//    server_addr.sin_family = IPv4;
//    server_addr.sin_addr.s_addr = INADDR_ANY;
//    server_addr.sin_port = htons(PORT);
//
//    bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
//    listen(server_socket, 5);
//    cout << "[Server started on port " << PORT << "]" << endl;
//
//    while (true) {
//        sockaddr_in client_addr {};
//        int client_size = sizeof(client_addr);
//        SOCKET client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &client_size);
//
//        if (client_socket != INVALID_SOCKET) {
//            thread t(handleClient, client_socket);
//            t.detach();
//        }
//    }
//
//    closesocket(server_socket);
//    WSACleanup();
//    return 0;
//}
