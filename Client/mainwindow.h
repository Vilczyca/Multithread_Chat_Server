#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include <thread>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")

#include <QMessageBox>
#include <iostream>

using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& username, SOCKET socket, const QStringList& users, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendAll(QString message);
    void sendPriv(QString user, QString message);
    void recvFromOther(QString user, QString message);
    void recvFromWisper(QString user, QString message);
    void recvFromServer(QString message);
    void on_button_send_message_clicked();

private:
    QString username;
    QStringList userList;
    SOCKET s;

    Ui::MainWindow *ui;
    std::thread recvThread;
    std::atomic<bool> running;
    void receiveLoop(); // funkcja do odbierania w tle
};

#endif // MAINWINDOW_H
