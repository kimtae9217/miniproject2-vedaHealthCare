#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

signals:
    void backButtonClicked();
    void registrationSuccessful(); // 회원가입 성공 시그널

private slots:
    void onRegisterClicked();
    void onBackButtonClicked();
    void onConnected();
    void onReadyRead();

private:
    Ui::login *ui;
    QTcpSocket *tcpSocket;
};

#endif // SIGNIN_H
