#include "mainwindow.h"
#include "./ui_mainwindow.h"

QString colorMsgMe = "#FF9898";
QString colorMsgOthers = "#FFAAAA";
QString colorMsgWhisper = "#FFE99A";
QString colorMsgWhisperSend = "#FFD586";


MainWindow::MainWindow(const QString& username, SOCKET socket, const QStringList& users, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    s(socket),
    username(username),
    running(true)
{
    ui->setupUi(this);
    ui->list_users->addItem("[Ty] " + username);

    for (const QString& user : users) {
        if (user != username)
            ui->list_users->addItem(user);
    }

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

void MainWindow::sendAll(QString message) {
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

    message = "/all " + message;
    cout << message.toStdString() << endl;

    send(s, message.toUtf8().constData(), message.length(), 0);
}

void MainWindow::sendPriv(QString user, QString message) {
    QWidget *messageWidget = new QWidget();
    messageWidget->setStyleSheet("background-color: " + colorMsgWhisperSend + "; border-radius: 5px;");

    QLabel *messageLabel = new QLabel(message);
    messageLabel->setStyleSheet("color: white;");
    messageLabel->setWordWrap(true);

    QLabel *usernameLabel = new QLabel(username + "->" + user);
    usernameLabel->setStyleSheet("color: gray;");
    usernameLabel->setWordWrap(true);

    QVBoxLayout *messageLayout = new QVBoxLayout();
    messageLayout->addWidget(messageLabel);
    messageWidget->setLayout(messageLayout);


    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->addWidget(usernameLabel);
    containerLayout->addWidget(messageWidget);
    // containerLayout->setSpacing(0);

    QWidget *containerWidget = new QWidget();

    containerWidget->setLayout(containerLayout);

    QListWidgetItem *item = new QListWidgetItem(ui->list_chat);
    item->setSizeHint(containerWidget->sizeHint());

    ui->list_chat->addItem(item);
    ui->list_chat->setItemWidget(item, containerWidget);

    message = "/whisp " + user + " " + message;
    cout << message.toStdString() << endl;

    send(s, message.toUtf8().constData(), message.length(), 0);
}



void MainWindow::recvFromOther(QString user, QString message)
{

    QWidget *messageWidget = new QWidget();
    messageWidget->setStyleSheet("background-color: " + colorMsgOthers + "; border-radius: 5px;");

    QLabel *messageLabel = new QLabel(message);
    messageLabel->setStyleSheet("color: white;");
    messageLabel->setWordWrap(true);

    QLabel *usernameLabel = new QLabel(user);
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


void MainWindow::recvFromWisper(QString user, QString message)
{

    QWidget *messageWidget = new QWidget();
    messageWidget->setStyleSheet("background-color: " + colorMsgWhisper + "; border-radius: 5px;");

    QLabel *messageLabel = new QLabel(message);
    messageLabel->setStyleSheet("color: white; ");
    messageLabel->setWordWrap(true);

    QLabel *usernameLabel = new QLabel(user);
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

void MainWindow::recvFromServer(QString message)
{
    QLabel *label = new QLabel(message);
    label->setStyleSheet("color: gray;");
    label->setWordWrap(true);

    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->addWidget(label);
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
    QString message=ui->lineedit_message->text();
    if (message == "") {
        return;
    }
    ui->lineedit_message->setText("");

    if (message.startsWith("/whisp ")) {
        QString user = message.section(' ', 1, 1);
        QString content = message.section(' ', 2);
        if (user.isEmpty() || content.isEmpty()) {
            recvFromServer("Nieprawidłowa składnia: użyj /whisp <użytkownik> <wiadomość>");
            return;
        }
        sendPriv(user, content);
    } else {
        sendAll(message);
    }

}

void MainWindow::receiveLoop() {
    char buffer[1024];

    while (running) {
        int bytes = recv(s, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            running = false;
            break;
        }
        buffer[bytes] = '\0';
        QString msg = QString::fromUtf8(buffer).trimmed();

        cout<<msg.toStdString()<<endl;


        QMetaObject::invokeMethod(this, [this, msg]() {
            QString user = msg.section("[", 1, 1).section("]", 0, 0);

            if (msg.startsWith("s")) {
                if (username == "SERVER") {
                    recvFromServer(msg.section(": ", 1));
                } else if (username != user) {
                    recvFromOther(user, msg.section(": ", 1));
                }
            } else if (msg.startsWith("p")) {
                if (user != username) {
                    recvFromWisper(user, msg.section(": ", 1));
                }

            } else if (msg.startsWith("n")) {
                recvFromServer(msg.section(": ", 1));
                QString afterPrefix = msg.section(": ", 1);
                QString userNotServer = afterPrefix.section(' ', 0, 0);
                if (userNotServer != username && userNotServer != "SERVER") {
                    ui->list_users->addItem(userNotServer);
                }
            } else if (msg.startsWith("x")) {
                recvFromServer(msg.section(": ", 1));
                QList<QListWidgetItem*> items = ui->list_users->findItems(user, Qt::MatchExactly);
                for (QListWidgetItem* item : items) {
                    delete ui->list_users->takeItem(ui->list_users->row(item));
                }
            } else {
                cout << "A co to tutaj mamy?\n" << msg.toStdString() << endl;
            }
        });
    }
}

// TODO:
// - Wyślij z enter