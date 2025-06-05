#ifndef USERNAMEWINDOW_H
#define USERNAMEWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class UsernameWindow;
}
QT_END_NAMESPACE

class UsernameWindow : public QMainWindow
{
    Q_OBJECT

public:
    UsernameWindow(QWidget *parent = nullptr);
    ~UsernameWindow();
private:
    Ui::UsernameWindow *ui;
};
#endif // USERNAMEWINDOW_H
