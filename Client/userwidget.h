#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include <winsock2.h>
# include <iostream>
#pragma comment(lib, "ws2_32.lib")
using namespace  std;

class UsernameDialog : public QDialog {
    Q_OBJECT

public:
    explicit UsernameDialog(SOCKET socket, QWidget *parent = nullptr);
    QString getUsername() const;
    QStringList getUserList() const;

    private slots:
        void acceptUsername();

private:
    QStringList userList;
    QLineEdit *usernameInput;
    QPushButton *confirmButton;
    QString username;
    SOCKET socket_fd;
};

#endif // USERWIDGET_H
