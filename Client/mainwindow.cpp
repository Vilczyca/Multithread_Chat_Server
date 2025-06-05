#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

using namespace std;

QString colorMsgMe = "#DB8DD0";
QString colorMsgOthers = "#FEC5F6";
QString colorMsgWhisper = "#FFDBDB";

QString usernamee;

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usernamee=username;
    ui->list_users->addItem("[Ty] "+ username);

    this->username = username;
    running = true;

    // Inicjalizacja Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        // QMessageBox::critical(this, "Błąd", "WSAStartup nieudane.");
        std::cout << "WSAStartup nieudane." << endl;
        exit(1);
    }
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        // QMessageBox::critical(this, "Błąd", "Nie udało się utworzyć gniazda.");
        std::cout << "Nie udało się utworzyć gniazda." << endl;
        WSACleanup();
        exit(1);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (::connect(s, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0) {
        // QMessageBox::critical(this, "Błąd", "Połączenie z serwerem nieudane.");
        std::cout << "Połączenie z serwerem nieudane." << endl;
        closesocket(s);
        WSACleanup();
        exit(1);
    }

    send(s, username.toUtf8().constData(), username.length(), 0);

    // Start receiver thread
    recvThread = std::thread(&MainWindow::receiveLoop, this);

}

MainWindow::~MainWindow()
{
    running = false;
    if (recvThread.joinable())
        recvThread.join();
    shutdown(s, SD_BOTH);
    closesocket(s);
    WSACleanup();
    delete ui;
}

void MainWindow::sendFromMe() {
    QString message=ui->lineedit_message->text();
    if (message == "") {
        return;
    }

    ui->lineedit_message->setText("");
    QString username=usernamee;

    QWidget *messageWidget = new QWidget();
    messageWidget->setStyleSheet("background-color: " + colorMsgMe + "; border-radius: 5px;");

    QLabel *messageLabel = new QLabel(message);
    messageLabel->setStyleSheet("color: white;");
    messageLabel->setWordWrap(true);

    QLabel *usernameLabel = new QLabel(username);
    usernameLabel->setStyleSheet("color: gray;");
    usernameLabel->setWordWrap(true);

    QVBoxLayout *messageLayout = new QVBoxLayout();
    messageLayout->addWidget(messageLabel);
    messageWidget->setLayout(messageLayout);


    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->addWidget(usernameLabel);
    containerLayout->addWidget(messageWidget);
    // containerLayout->setSpacing(2);

    QWidget *containerWidget = new QWidget();

    containerWidget->setLayout(containerLayout);

    QListWidgetItem *item = new QListWidgetItem(ui->list_chat);
    item->setSizeHint(containerWidget->sizeHint());

    ui->list_chat->addItem(item);
    ui->list_chat->setItemWidget(item, containerWidget);
}


void MainWindow::sendFromOther()
{
    QString message="Hello, testowa wiadomosć.";
    QString username="WilczaPasjonatka2003";

    QWidget *messageWidget = new QWidget();
    messageWidget->setStyleSheet("background-color: " + colorMsgOthers + "; border-radius: 5px;");

    QLabel *messageLabel = new QLabel(message);
    messageLabel->setStyleSheet("color: white;");
    messageLabel->setWordWrap(true);

    QLabel *usernameLabel = new QLabel(username);
    usernameLabel->setStyleSheet("color: gray;");
    usernameLabel->setWordWrap(true);

    QVBoxLayout *messageLayout = new QVBoxLayout();
    messageLayout->addWidget(messageLabel);
    messageWidget->setLayout(messageLayout);


    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->addWidget(usernameLabel);
    containerLayout->addWidget(messageWidget);
    // containerLayout->setSpacing(2);

    QWidget *containerWidget = new QWidget();

    containerWidget->setLayout(containerLayout);

    QListWidgetItem *item = new QListWidgetItem(ui->list_chat);
    item->setSizeHint(containerWidget->sizeHint());

    ui->list_chat->addItem(item);
    ui->list_chat->setItemWidget(item, containerWidget);
}


void MainWindow::sendFromWisper()
{
    QString message="Psst, jestem Twoją cichą wielbicielką.";
    QString username="WilczaPasjonatka2003 (priv)";

    QWidget *messageWidget = new QWidget();
    messageWidget->setStyleSheet("background-color: " + colorMsgWhisper + "; border-radius: 5px;");

    QLabel *messageLabel = new QLabel(message);
    messageLabel->setStyleSheet("color: white; ");
    messageLabel->setWordWrap(true);

    QLabel *usernameLabel = new QLabel(username);
    usernameLabel->setStyleSheet("color: gray;");
    usernameLabel->setWordWrap(true);

    QVBoxLayout *messageLayout = new QVBoxLayout();
    messageLayout->addWidget(messageLabel);
    messageWidget->setLayout(messageLayout);


    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->addWidget(usernameLabel);
    containerLayout->addWidget(messageWidget);
    // containerLayout->setSpacing(2);

    QWidget *containerWidget = new QWidget();

    containerWidget->setLayout(containerLayout);

    QListWidgetItem *item = new QListWidgetItem(ui->list_chat);
    item->setSizeHint(containerWidget->sizeHint());

    ui->list_chat->addItem(item);
    ui->list_chat->setItemWidget(item, containerWidget);
}


void MainWindow::on_button_send_message_clicked()
{
    sendFromMe();
}



void MainWindow::on_pushButton_4_clicked()
{
    sendFromOther();
}


void MainWindow::on_pushButton_5_clicked()
{
    sendFromWisper();
}


// TODO:
// - Wyślij z enter