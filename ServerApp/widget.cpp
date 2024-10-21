#include "widget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtNetwork>
#include <QDebug>
#include <QtSql>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

Widget::Widget(QWidget *parent)
    : QWidget(parent), tcpServer(nullptr)
{
    statusLabel = new QLabel("'Start Server' 버튼을 눌러주세요!", this);
    startButton = new QPushButton("Start Server", this);
    stopButton = new QPushButton("Stop Server", this);

    connect(startButton, &QPushButton::clicked, this, &Widget::startServer);
    connect(stopButton, &QPushButton::clicked, this, &Widget::stopServer);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(statusLabel);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);

    setLayout(layout);
    setWindowTitle("Simple TCP Server");

    updateButtonStates();

    if (!initDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to initialize database.");
    }

}

QString Widget::getLocalIPAddress()
{

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // IPv4 주소 찾기
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 만약 IPv4 주소를 찾지 못했다면, localhost를 사용
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    return ipAddress;

    //return "127.0.0.1";
}


void Widget::startServer()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 8080)) {
        QMessageBox::critical(this, tr("TCP Server"),
                              tr("Unable to start the server: %1.")
                                  .arg(tcpServer->errorString()));
        delete tcpServer;
        tcpServer = nullptr;
        return;
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &Widget::clientConnect);
    statusLabel->setText("Server is running on port 8080");

    QString localIP = getLocalIPAddress();
    qDebug() << "Server started on" << localIP << ":" << tcpServer->serverPort();

    updateButtonStates();
}

void Widget::stopServer()
{
    if (tcpServer) {
        tcpServer->close();
        delete tcpServer;
        tcpServer = nullptr;
    }
    statusLabel->setText("Server is stopped");
    qDebug("server stopped");
    updateButtonStates();
}

// void Widget::clientConnect()
// {
//     QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

//     connect(clientSocket, &QAbstractSocket::disconnected, clientSocket, &QObject::deleteLater);
//     connect(clientSocket, &QAbstractSocket::disconnected,
//             [this, clientSocket]() { clientSockets.removeOne(clientSocket); });
//     connect(clientSocket, &QTcpSocket::readyRead,
//             [this, clientSocket]() { processClientData(clientSocket); });

//     clientSockets.append(clientSocket);
//     statusLabel->setText("New client connected");
//     qDebug() << "New client connected";

//     sendServerInfo();
// }

void Widget::clientConnect()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    // 고객 이름을 포함한 클라이언트 식별 정보 받기
    connect(clientSocket, &QTcpSocket::readyRead, [this, clientSocket]() {
        QByteArray data = clientSocket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject json = doc.object();

        if (json["type"].toString() == "REGISTER") {
            QString customerName = json["username"].toString();
            clientSockets[customerName] = clientSocket;  // 고객 이름을 통해 소켓을 저장

            QJsonObject response;
            response["type"] = "REGISTER_RESPONSE";
            response["success"] = true;

            QJsonDocument responseDoc(response);
            clientSocket->write(responseDoc.toJson());

            statusLabel->setText(customerName + " connected");
            qDebug() << customerName << " connected";
        }
    });

    connect(clientSocket, &QAbstractSocket::disconnected, clientSocket, &QObject::deleteLater);
}


void Widget::sendServerInfo()
{
    QString ipAddress = getLocalIPAddress();

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << QString("SERVER_INFO");
    out << ipAddress;
    out << tcpServer->serverPort();

    for (QTcpSocket *socket : clientSockets) {
        socket->write(data);
    }

    qDebug() << "Sent server info: IP" << ipAddress << ", Port" << tcpServer->serverPort();
}

void Widget::updateButtonStates()
{
    startButton->setEnabled(tcpServer == nullptr);
    stopButton->setEnabled(tcpServer != nullptr);
}

// 데이터베이스 연결 및 테이블 생성
bool Widget::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // 경로 설정 필수
    // MacOS는 경로설정을 해줘야 db가 저장됨
    db.setDatabaseName("/Users/jinurung/VEDA/Project/healthcare/vedahealthcare/miniproject2-vedaHealthCare/ServerApp/build/Qt_6_8_0_for_macOS-Debug//users.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users "
               "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "email TEXT, "
               "username TEXT, "
               "password TEXT)");

    return true;
}

// 사용자 등록 함수
bool Widget::registerUser(const QString &email, const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (email, username, password) "
                  "VALUES (:email, :username, :password)");
    query.bindValue(":email", email);
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    return query.exec();
}

// 클라이언트로부터 데이터를 받아 처리하는 함수
void Widget::processClientData(QTcpSocket *clientSocket)
{
    QByteArray data = clientSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if (json["type"].toString() == "REGISTER") {
        QString email = json["email"].toString();
        QString username = json["username"].toString();
        QString password = json["password"].toString();

        bool success = registerUser(email, username, password);

        QJsonObject response;
        response["type"] = "REGISTER_RESPONSE";
        response["success"] = success;

        QJsonDocument responseDoc(response);
        clientSocket->write(responseDoc.toJson());
    }
}

void Widget::sendMessageToClient(const QString& customerName, const QString& message)
{
    if (clientSockets.contains(customerName)) {
        QTcpSocket *socket = clientSockets[customerName];
        QJsonObject json;
        json["type"] = "MESSAGE";
        json["message"] = message;

        QJsonDocument doc(json);
        socket->write(doc.toJson());
    }
}
