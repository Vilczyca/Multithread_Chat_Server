#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class UsernameDialog : public QDialog {
    Q_OBJECT

public:
    explicit UsernameDialog(QWidget *parent = nullptr);
    QString getUsername() const;

private slots:
    void acceptUsername();

private:
    QLineEdit *usernameInput;
    QPushButton *confirmButton;
    QString username;
};


#endif // USERWIDGET_H
