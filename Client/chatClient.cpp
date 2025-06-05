#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")

using namespace std;

const int BUFFER_SIZE = 1024;
const string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 5555;

atomic<bool> is_running{true};
SOCKET client_socket;
mutex queue_mutex;
queue<string> message_queue;


HWND hEditChat;
HWND hEditInput;
HWND hBtnSend;
HWND hEditUsers;
HWND hBtnPrivate;



void ReceiverThread() {
    char buffer[BUFFER_SIZE];
    while (is_running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            PostMessage(GetParent(hEditChat), WM_APP + 1, 0, 0); // Powiadomienie o rozłączeniu
            break;
        }

        std::lock_guard<std::mutex> lock(queue_mutex);
        message_queue.push("[Serwer]: " + std::string(buffer));
        InvalidateRect(hEditChat, NULL, TRUE); // Odśwież okno
    }
}

void SendMessageToServer() {
    char buffer[1024];
    GetWindowTextA(hEditInput, buffer, 1024);

    if (strlen(buffer) > 0) {
        send(client_socket, buffer, strlen(buffer), 0);
        SetWindowTextA(hEditInput, "");

        std::lock_guard<std::mutex> lock(queue_mutex);
        message_queue.push("[Ty]: " + std::string(buffer));
        InvalidateRect(hEditChat, NULL, TRUE);
    }
}

void SendPrivateMessage() {
    char recipient[256], message[1024];

    // Pobierz wybranego użytkownika z listy
    int sel = SendMessage(hEditUsers, LB_GETCURSEL, 0, 0);
    SendMessage(hEditUsers, LB_GETTEXT, sel, (LPARAM)recipient);

    // Pobierz treść wiadomości
    GetWindowTextA(hEditInput, message, 1024);

    if (strlen(recipient) && strlen(message)) {
        std::string full_msg = "/msg " + std::string(recipient) + " " + std::string(message);
        send(client_socket, full_msg.c_str(), full_msg.size(), 0);
    }
}

void UpdateUsersList(const std::string& user_list) {
    SendMessage(hEditUsers, LB_RESETCONTENT, 0, 0);

    std::istringstream iss(user_list);
    std::string user;
    while (iss >> user) {
        SendMessageA(hEditUsers, LB_ADDSTRING, 0, (LPARAM)user.c_str());
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) SendMessageToServer(); // Kliknięto przycisk
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hEditChat, &ps);

            std::lock_guard<std::mutex> lock(queue_mutex);
            int yPos = 10;
            std::queue<std::string> temp = message_queue;

            while (!temp.empty()) {
                TextOutA(hdc, 10, yPos, temp.front().c_str(), temp.front().length());
                yPos += 20;
                temp.pop();
            }

            EndPaint(hEditChat, &ps);
            break;
        }

        case WM_DESTROY:
            is_running = false;
            closesocket(client_socket);
            WSACleanup();
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inicjalizacja Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Tworzenie okna
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ChatClient";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("ChatClient", "Chat Client", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 800, 300, NULL, NULL, hInstance, NULL);

    // Kontrolki
    hEditChat = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
                             10, 10, 360, 200, hwnd, NULL, hInstance, NULL);

    hEditInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                              10, 220, 280, 25, hwnd, NULL, hInstance, NULL);

    hBtnSend = CreateWindow("BUTTON", "Send", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                            300, 220, 70, 25, hwnd, (HMENU)1, hInstance, NULL);

    // Połączenie z serwerem
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));

    // Uruchomienie wątku odbierającego
    std::thread(ReceiverThread).detach();

    // Pętla komunikatów
    ShowWindow(hwnd, nCmdShow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}