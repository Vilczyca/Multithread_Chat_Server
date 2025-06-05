#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include <thread>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendFromMe();
    void sendFromOther();
    void sendFromWisper();


    void on_button_send_message_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    QString username;
    SOCKET s;

    Ui::MainWindow *ui;
    std::thread recvThread;
    std::atomic<bool> running;
    void receiveLoop(); // funkcja do odbierania w tle
};

#endif // MAINWINDOW_H
