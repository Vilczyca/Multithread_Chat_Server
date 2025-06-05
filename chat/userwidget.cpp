#include "userwidget.h"
#include <QVBoxLayout>

UsernameDialog::UsernameDialog(QWidget *parent) : QDialog(parent) {
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
    if (!username.isEmpty()) {
        accept();
    }
}

QString UsernameDialog::getUsername() const {
    return username;
}
