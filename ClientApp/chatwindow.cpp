#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QJsonDocument>  // QJsonDocument 헤더 추가
#include <QJsonObject>    // QJsonObject 헤더 추가
#include <QByteArray>     // QByteArray 헤더 추가


ChatWindow::ChatWindow(QWidget *parent, const QString& customerName)
    : QDialog(parent)
    , ui(new Ui::ChatWindow)
    , socket(nullptr)  // 소켓 초기화
    , customerName(customerName)  // 고객 이름 초기화
{
    ui->setupUi(this);

    // 윈도우 제목에 고객 이름 표시
    setWindowTitle("Chat with " + customerName);

    // 서버에 연결
    connectToServer();

    // 전송 버튼 클릭 시 메시지 보내기
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatWindow::onSendMessage);
}

ChatWindow::~ChatWindow()
{
    delete ui;
    if (socket) {
        socket->close();
        delete socket;
    }
}

// 서버에 연결하는 함수
void ChatWindow::connectToServer()
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, [this]() {
        ui->chatLog->append("서버에 연결되었습니다.");
    });

    connect(socket, &QTcpSocket::disconnected, [this]() {
        ui->chatLog->append("서버와의 연결이 끊어졌습니다.");
    });

    connect(socket, &QTcpSocket::errorOccurred, [this](QAbstractSocket::SocketError socketError) {
        ui->chatLog->append("연결 오류: " + socket->errorString());
    });

    connect(socket, &QTcpSocket::readyRead, this, &ChatWindow::onMessageReceived);

    socket->connectToHost("127.0.0.1", 8080);  // 서버 IP와 포트 설정
}

// 메시지를 받는 함수
// void ChatWindow::onMessageReceived()
// {
//     QString message = socket->readAll();
//     ui->chatLog->append(customerName + ": " + message);  // 고객 이름과 함께 메시지 출력
// }

void ChatWindow::onMessageReceived()
{
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if (json["type"].toString() == "MESSAGE") {
        QString message = json["message"].toString();
        ui->chatLog->append(customerName + ": " + message);
    }
}

// 메시지를 보내는 함수
// void ChatWindow::onSendMessage()
// {
//     QString message = ui->messageInput->text();  // 입력 필드에서 메시지 가져오기
//     if (!message.isEmpty()) {
//         socket->write(message.toUtf8());
//         ui->chatLog->append("You: " + message);  // 전송된 메시지 로깅
//         ui->messageInput->clear();  // 입력 필드 초기화
//     }
// }

void ChatWindow::onSendMessage()
{
    QString message = ui->messageInput->text();
    if (!message.isEmpty()) {
        QJsonObject json;
        json["type"] = "MESSAGE";
        json["message"] = message;

        QJsonDocument doc(json);
        socket->write(doc.toJson());

        ui->chatLog->append("You: " + message);
        ui->messageInput->clear();
    }
}
