#include <winsock2.h>
#include <iostream>

#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <algorithm>
#include <sstream>

#define IPv4 AF_INET
#define TCP SOCK_STREAM

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5555;

struct Client {
    string username = "Anonymous";
    SOCKET socket;
};

vector<Client> clients;
mutex clients_mutex;

// Check if username is already taken

bool checkUsername(const string& username) {

    if (username == "SERVER" || username == "ADMIN")
        return  false;

    for (const Client& client : clients) {
        if (client.username == username) {
            return false;
        }
    }
    return true;
}


// Broadcast to all clients except sender

void sendToAll(const string& msg) {
    lock_guard<mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        string finalMsg = "s ";
        finalMsg.append(msg);
        send(client.socket, finalMsg.c_str(), static_cast<int>(finalMsg.length()), 0);
    }
}

void sendToClient(const string& msg, SOCKET sender_socket,
                  const string& username) {\
                  bool foundUser = false;
    lock_guard<mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        if (client.username == username) {
            string finalMsg = "p ";
            finalMsg.append(msg);
            send(client.socket, finalMsg.c_str(), static_cast<int>(finalMsg.length()), 0);
            send(sender_socket, finalMsg.c_str(), static_cast<int>(finalMsg.length()), 0);

            foundUser = true;
            break;
        }
    }
    if (!foundUser) {
        string errorMsg =
                "p [SERVER]: Nie udało dostarczyć się wiadomości :<";
        send(sender_socket, errorMsg.c_str(),
             static_cast<int>(errorMsg.length()), 0); }
}

// Handle individual client
void handleClient(SOCKET client_socket) {
    char buffer[1024] = {};
    string username = "Anonymous";
    string msg;
    bool usernameAccepted = false;

    while (!usernameAccepted) {
        int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) {
            closesocket(client_socket);
            return;
        }

        buffer[received] = '\0';
        username = buffer;
        {
            lock_guard<mutex> lock(clients_mutex);
            if (!checkUsername(username)) {
                msg = "NOPE";
                send(client_socket, msg.c_str(), static_cast<int>(msg.size()), 0);
                continue;
            }

            clients.push_back(Client{username, client_socket});
            usernameAccepted = true;
            msg = "YUP";
            send(client_socket, msg.c_str(), static_cast<int>(msg.size()), 0);
        }
    }

    // receive username


    msg = "[SERVER]: " + username + " dołącza do czatu! Hurra!\n";
    cout << msg;
    sendToAll(msg);
    msg = "p [SERVER]: Dołączasz do serwera !";
    send(client_socket,msg.c_str(),static_cast<int>(msg.size()), 0);


    while (true) {
        // recive message
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket,
                                  buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) break;

        // response
        buffer[bytes_received] = '\0';
        string action = "/all";
        istringstream iss(buffer);
        iss >> action;

        if (action == "/whisp") {
            string fromUser = "Anon";
            iss >> fromUser;
            getline(iss, msg);
            string finalMsg = "[" + username + "]:";
            finalMsg.append(msg);
            sendToClient(finalMsg, client_socket, fromUser);
        } else if (action == "/all") {
            getline(iss, msg);
            string finalMsg = "[" + username + "]:";
            finalMsg.append(msg);
            cout << msg << endl;
            sendToAll(finalMsg);
        } else if (action == "/exit") {
            {
                lock_guard<mutex> lock(clients_mutex);
                clients.erase(remove_if(clients.begin(), clients.end(),
                                        [&](const Client& c) {
                                            return c.socket == client_socket;
                                        }), clients.end());
            }
            msg = "[SERVER]: " + username + " poszedł sobie.\n";
            cout << msg;
            sendToAll(msg);
            closesocket(client_socket);
        } else {
            msg = "p [SERVER] >>> Nieznane polecenie!";
            send(client_socket, msg.c_str(), static_cast<int>(msg.size()), 0);
        }
    }
}

int main() {
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    SOCKET server_socket = socket(IPv4, TCP, 0);

    sockaddr_in server_addr {};
    server_addr.sin_family = IPv4;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int bindCode = bind(server_socket, (SOCKADDR*)&server_addr,
                        sizeof(server_addr));

    if (bindCode == SOCKET_ERROR)
    {
        cerr << "[Server socket error with code " << bindCode << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    listen(server_socket, 5);
    cout << "[Server started on port " << PORT << "]" << endl;

    while (true) {
        sockaddr_in client_addr {};
        int client_size = sizeof(client_addr);
        SOCKET client_socket = accept(
                server_socket, (SOCKADDR*)&client_addr, &client_size);

        if (client_socket != INVALID_SOCKET) {
            thread t(handleClient, client_socket);
            t.detach();
        }
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
