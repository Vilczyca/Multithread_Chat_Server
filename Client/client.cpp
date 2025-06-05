// #include <iostream>
// #include <winsock2.h>
// #include <thread>
// #include <atomic>
//
// #define IPv4 AF_INET
// #define TCP SOCK_STREAM
//
// #pragma comment(lib, "ws2_32.lib")
//
// using namespace std;
//
// const char* SERVER_IP = "127.0.0.1";
// const int PORT = 5555;
//
// atomic<bool> running(true);
//
// // Receiver function
// void receiveMessages(SOCKET socket) {
//     char buffer[1024];
//     int bytes_received;
//
//     while (running) {
//         bytes_received = recv(socket, buffer, sizeof(buffer) - 1, 0);
//         if (bytes_received <= 0) {
//             cerr << "[Disconnected or error receiving.]" << endl;
//             running = false;
//             break;
//         }
//         buffer[bytes_received] = '\0';
//         cout << "\n[Server]: " << buffer << "\n> ";
//         cout.flush();
//     }
// }
//
// // Sender function
// void sendMessages(SOCKET socket) {
//     string msg;
//     while (running && getline(cin, msg)) {
//         if (msg == "/exit") {
//             running = false;
//             break;
//         }
//         send(socket, msg.c_str(), msg.length(), 0);
//     }
// }
//
//
// int main() {
//     WSADATA wsa_data;
//     if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
//         cerr << "WSA Initialization failed." << endl;
//         return 1;
//     }
//
//     SOCKET client_socket = socket(IPv4, TCP, 0);
//     if (client_socket == INVALID_SOCKET) {
//         cerr << "Socket creation failed." << endl;
//         WSACleanup();
//         return 1;
//     }
//
//     sockaddr_in server_addr {};
//     server_addr.sin_family = IPv4;
//     server_addr.sin_port = htons(PORT);
//     server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
//
//     if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) != 0) {
//         cerr << "Connection error." << endl;
//         closesocket(client_socket);
//         WSACleanup();
//         return 1;
//     }
//
//     cout << "[Connected with server!]" << endl;
//     cout << "Choose your username: ";
//
//     // Start receiver and sender threads
//     thread recv_thread(receiveMessages, client_socket);
//     thread send_thread(sendMessages, client_socket);
//
//     send_thread.join();
//     running = false; // signal the receive thread to stop
//     shutdown(client_socket, SD_BOTH);
//     closesocket(client_socket);
//     recv_thread.join();
//
//     WSACleanup();
//     return 0;
// }

#include "../chat/mainwindow.h"
// #include "../chat/userwidget.h"
#include "userwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UsernameDialog usernameDialog;
    if (usernameDialog.exec() == QDialog::Accepted) {
        QString username = usernameDialog.getUsername();

        // Tworzymy i przekazujemy username do MainWindow
        MainWindow mainWindow(username);
        mainWindow.show();

        return app.exec();
    }

    return 0;
    // MainWindow w;
    // w.show();
    // return a.exec();
}
