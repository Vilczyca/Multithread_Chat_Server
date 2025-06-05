#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendFromMe() {
    QString message=ui->lineedit_message->text();
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

