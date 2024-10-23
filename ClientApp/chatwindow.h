#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr, const QString& customerName = "");
    ~ChatWindow();

private slots:
    void connectToServer();  // 서버에 연결하는 함수
    void onMessageReceived();  // 메시지를 받는 함수
    void onSendMessage();  // 메시지를 보내는 함수

private:
    Ui::ChatWindow *ui;
    QTcpSocket *socket;  // 서버와의 소켓 연결
    QString customerName;  // 현재 채팅 중인 고객 이름
};

#endif // CHATWINDOW_H
