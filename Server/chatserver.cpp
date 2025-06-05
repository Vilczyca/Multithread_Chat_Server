#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <queue>
#include <vector>
#include <atomic>
#include <cstring>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5555;
const int MAX_CLIENTS = 100;
const int BUFFER_SIZE = 1024;

struct User {
    string login;
    string password;
    bool is_online = false;
    SOCKET socket;
    queue<std::string> messages;
};

unordered_map<std::string, User> users_db;
unordered_map<SOCKET, User*> active_users;
mutex db_mutex;
atomic<bool> server_running{true};

string hash_password(const string& pass) {

    hash<string> hasher;
    return to_string(hasher(pass + "S@lt!"));
}

bool is_valid_login(const string& login) {
    return login.find(' ') == string::npos &&
           login.size() >= 3 &&
           login.size() <= 20;
}

void broadcast(const std::string& msg, SOCKET exclude = INVALID_SOCKET) {
    std::lock_guard<std::mutex> lock(db_mutex);
    for (auto& [socket, user] : active_users) {
        if (socket != exclude) {
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
}

void send_user_list(SOCKET target) {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::string list = "/users";
    for (auto& [login, user] : users_db) {
        if (user.is_online) {
            list += " " + login;
        }
    }
    send(target, list.c_str(), list.size(), 0);
}



void process_command(SOCKET client_socket, const std::string& cmd) {
    std::istringstream iss(cmd);
    std::string command;
    iss >> command;

    if (command == "/register") {
        std::string login, password;
        iss >> login >> password;

        if (!is_valid_login(login)) {
            send(client_socket, "Error: Invalid login", 19, 0);
            return;
        }

        std::lock_guard<std::mutex> lock(db_mutex);
        if (users_db.count(login)) {
            send(client_socket, "Error: User exists", 18, 0);
        } else {
            users_db[login] = {login, hash_password(password), true, client_socket};
            active_users[client_socket] = &users_db[login];
            send(client_socket, "Registered successfully", 23, 0);
            broadcast(login + " joined the chat", client_socket);
            send_user_list(client_socket);
        }
    }
    else if (command == "/login") {
        std::string login, password;
        iss >> login >> password;

        std::lock_guard<std::mutex> lock(db_mutex);
        if (!users_db.count(login) || users_db[login].password != hash_password(password)) {
            send(client_socket, "Error: Invalid credentials", 26, 0);
        } else if (users_db[login].is_online) {
            send(client_socket, "Error: Already logged in", 24, 0);
        } else {
            users_db[login].is_online = true;
            users_db[login].socket = client_socket;
            active_users[client_socket] = &users_db[login];
            send(client_socket, "Welcome back!", 13, 0);
            broadcast(login + " reconnected", client_socket);
            send_user_list(client_socket);

            // Wysyłanie zaległych wiadomości
            while (!users_db[login].messages.empty()) {
                send(client_socket, users_db[login].messages.front().c_str(),
                     users_db[login].messages.front().size(), 0);
                users_db[login].messages.pop();
            }
        }
    }
    else if (command == "/msg") {
        std::string recipient, message;
        iss >> recipient;
        getline(iss, message); //Reszta to wiadomość

        std::lock_guard<std::mutex> lock(db_mutex);
        if (!users_db.count(recipient)) {
            send(client_socket, "Error: User not found", 21, 0);
        } else {
            std::string sender = active_users[client_socket]->login;
            std::string full_msg = "[PM from " + sender + "]: " + message;

            if (users_db[recipient].is_online) {
                send(users_db[recipient].socket, full_msg.c_str(), full_msg.size(), 0);
            } else {
                users_db[recipient].messages.push(full_msg);
            }
            send(client_socket, "Message sent", 12, 0);
        }
    }
    else if (command == "/users") {
        send_user_list(client_socket);
    }
    else {
        // Normalna wiadomość czatu
        std::string sender = active_users[client_socket]->login;
        broadcast(sender + ": " + cmd, client_socket);
    }
}

void client_handler(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];

    while (server_running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        process_command(client_socket, std::string(buffer, bytes_received));
    }

    // Czyszczenie po rozłączonym kliencie
    std::lock_guard<std::mutex> lock(db_mutex);
    if (active_users.count(client_socket)) {
        std::string login = active_users[client_socket]->login;
        users_db[login].is_online = false;
        active_users.erase(client_socket);
        broadcast(login + " left the chat", client_socket);
    }
    closesocket(client_socket);
}


int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server running on port " << PORT << std::endl;

    while (server_running) {
        sockaddr_in client_addr{};
        int client_len = sizeof(client_addr);
        SOCKET client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);

        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::thread(client_handler, client_socket).detach();
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
