#include <winsock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <algorithm>
#include <sstream>
#include <random>

#define IPv4 AF_INET
#define TCP SOCK_STREAM

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5555;
SOCKET server_socket = INVALID_SOCKET;

struct Client {
    string username = "Anonymous";
    SOCKET socket;
};


vector<Client> clients;
mutex clients_mutex;
atomic<bool> running(true);
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distr(0, 3);

bool checkUsername(const string& username) {

    if (username == "SERVER" || username == "ADMIN")
        return  false;

    return std::ranges::all_of(clients, [&username](const Client& client) {
        return client.username != username;
    });
}

void sendOnStatus(const string& msg, SOCKET sender_socket) {
    lock_guard<mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        if (client.socket != sender_socket)
            send(client.socket, msg.c_str(), static_cast<int>(msg.length()), 0);
    }
}

void sendToAll(const string& msg) {
    lock_guard<mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        string finalMsg = "s ";
        finalMsg.append(msg);
        send(client.socket, finalMsg.c_str(),
             static_cast<int>(finalMsg.length()), 0);
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

void handleClient(SOCKET client_socket) {
    char buffer[1024] = {};
    string welcomeMessage[4] = {" dołącza do serwera!!! HURRA!",
                                " właśnie wbija na czat... Ratuj się kto może!",
                                " dołączył! Szykuje się dobra zabawa ;)",
                                " pojawia się! I liczy na gromkie powitanie!"};
    string leaveMessage[4] = {" ucieka! Szybko, rozpocząć pościg!",
                              " idzie szukać wiatru w polu... Będziemy tęsknić !",
                              " przenosi się do innego wymiaru! Ciekawe czy kiedyś powróci...",
                              " został porwany! Ratujcie, szybko!"};
    string username = "Anonymous";
    string msg;
    bool usernameAccepted = false;
    bool clientActive = true;

    if (client_socket == INVALID_SOCKET)
        clientActive = false;

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
            msg = "YUP ";
            msg.append(to_string(clients.size()));
            for (const Client& c : clients)
            {
                msg.append(" ").append(c.username);
            }
            send(client_socket, msg.c_str(), static_cast<int>(msg.size()), 0);
        }
    }

    int randNumber = distr(gen);
    msg = "n [SERVER]: " + username + welcomeMessage[randNumber];
    cout << msg << endl;
    sendOnStatus(msg,client_socket);
    send(client_socket, msg.c_str(), static_cast<int>(msg.size()), 0);



    while (clientActive) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket,
                                  buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) {
            clientActive = false;
            break;
        }

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
           clientActive = false;
        } else {
            msg = "p [SERVER] >>> Nieznane polecenie!";
            send(client_socket, msg.c_str(), static_cast<int>(msg.size()), 0);
        }
    }

    {
        lock_guard<mutex> lock(clients_mutex);
        clients.erase(remove_if(clients.begin(), clients.end(),
                                [&](const Client& c) {
                                    return c.socket == client_socket;
                                }), clients.end());
    }
    randNumber = distr(gen);
    msg = "x [SERVER]: " + username + leaveMessage[randNumber];
    cout << msg << endl;
    sendOnStatus(msg,client_socket);
    closesocket(client_socket);
}

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
        cout << ">>> [Server shutting down...]" << endl;
        running = false;

        if (server_socket != INVALID_SOCKET) {
            closesocket(server_socket);
            server_socket = INVALID_SOCKET;
        }
    }
    return TRUE;
}

void commandListener() {
    string command;
    while (running) {
        getline(cin, command);
        if (command == "exit") {
            cout << ">>> [Server shutting down...]" << endl;
            running = false;

            if (server_socket != INVALID_SOCKET) {
                closesocket(server_socket);
                server_socket = INVALID_SOCKET;
            }
            break;
        }
    }
}

int main() {
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        std::cerr << ">>> Error setting console handler" << std::endl;
        return 1;
    }
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        cerr << ">>> WSAStartup failed with error: " << WSAGetLastError() << endl;
        return 1;
    }

//    SOCKET server_socket = socket(IPv4, TCP, 0);
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        cerr << ">>> socket failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << ">>> bind failed with error: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << ">>> listen failed with error: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    cout << ">>> [Server started on port " << PORT << "]" << endl;
    cout << ">>> Type 'exit' to shutdown server" << std::endl;

    thread cmd_thread(commandListener);
    cmd_thread.detach();

    while (running) {
        sockaddr_in client_addr {};
        int client_size = sizeof(client_addr);
        SOCKET client_socket = accept(
                server_socket, (SOCKADDR*)&client_addr, &client_size);

        if (client_socket == INVALID_SOCKET) {
            if (running) {
                cerr << ">>> accept failed with error: " << WSAGetLastError() << endl;
            }
            continue;
        }

        if (client_socket != INVALID_SOCKET) {
            thread t(handleClient, client_socket);
            t.detach();
        }
    }
    cout << ">>> Closing all client connections..." << endl;
    {
        lock_guard<mutex> lock(clients_mutex);
        for (const Client& c : clients) {
            if (c.socket != INVALID_SOCKET) {
                closesocket(c.socket);
            }
        }
    }

    closesocket(server_socket);
    WSACleanup();

    cout << ">>> [Server stopped] <<<" << endl;

    return 0;
}
