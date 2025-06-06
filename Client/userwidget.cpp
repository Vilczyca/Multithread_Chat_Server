#include "userwidget.h"
#include <QVBoxLayout>

UsernameDialog::UsernameDialog(SOCKET socket, QWidget *parent)
    : QDialog(parent), socket_fd(socket)
{
    setWindowTitle("Podaj nazwę użytkownika");

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Wpisz swoją nazwę...");

    confirmButton = new QPushButton("OK", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(usernameInput);
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, this, &UsernameDialog::acceptUsername);
}

void UsernameDialog::acceptUsername() {
    username = usernameInput->text();
    if (username.isEmpty())
        return;

    // send username to server
    std::string nameStr = username.toStdString();
    send(socket_fd, nameStr.c_str(), static_cast<int>(nameStr.length()), 0);

    char buffer[1024] = {};
    int bytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        qWarning("Błąd przy odbiorze odpowiedzi od serwera");
        return;
    }
    buffer[bytes] = '\0';
    QString response = QString::fromUtf8(buffer);

    if (response.startsWith("YUP")) {
        QStringList parts = response.split(' ', Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
            parts.removeFirst();
            parts.removeFirst();
            userList = parts; // only usernames
        }
        accept();
    } else if (response.startsWith("NOPE")) {
        QMessageBox::warning(this, "Błąd", "Nazwa użytkownika jest zajęta lub zabroniona.");
    } else {
        QMessageBox::warning(this, "Błąd", "Nieznana odpowiedź od serwera.");
    }
}

QString UsernameDialog::getUsername() const {
    return username;
}

QStringList UsernameDialog::getUserList() const {
    return userList;
}

