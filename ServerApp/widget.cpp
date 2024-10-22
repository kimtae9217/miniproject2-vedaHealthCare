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


void Widget::clientConnect()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, &QAbstractSocket::disconnected, clientSocket, &QObject::deleteLater);
    connect(clientSocket, &QTcpSocket::readyRead,
            [this, clientSocket]() { processClientData(clientSocket); });

    statusLabel->setText("New client connected");
    qDebug() << "New client connected";
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
    //db.setDatabaseName("/Users/jinurung/VEDA/Project/healthcare/vedahealthcare/miniproject2-vedaHealthCare/ServerApp/build/Qt_6_8_0_for_macOS-Debug//users.db");
    db.setDatabaseName("/Users/taewonkim/GitHub/miniproject2-vedaHealthCare/ServerApp/build/Qt_6_7_2_for_macOS-Debug/users.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users "
               "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT, "
               "age TEXT, "
               "email TEXT, "
               "userid TEXT, "
               "password TEXT)");

    return true;
}

// 사용자 등록 함수
bool Widget::registerUser(const QString &name, const QString &age, const QString &email, const QString &userid, const QString &password)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (name, age, email, userid, password) "
                  "VALUES (:name, :age, :email, :userid, :password)");
    query.bindValue(":name", name);
    query.bindValue(":age", age);
    query.bindValue(":email", email);
    query.bindValue(":userid", userid);
    query.bindValue(":password", password);

    bool success = query.exec();
    if (!success) {
        qDebug() << "SQL Error:" << query.lastError().text();
    }
    return success;

    // return query.exec();
}

// 클라이언트로부터 데이터를 받아 처리하는 함수
void Widget::processClientData(QTcpSocket *clientSocket)
{
    QByteArray data = clientSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    qDebug() << "Received data:" << doc.toJson();

    if (json["type"].toString() == "REGISTER") {
        QString name = json["name"].toString();
        QString age = json["age"].toString();
        QString email = json["email"].toString();
        QString userid = json["userid"].toString();
        QString password = json["password"].toString();

        qDebug() << "Registering user:" << name << age << email << userid;

        bool success = registerUser(name, age, email, userid, password);

        qDebug() << "Registration" << (success ? "successful" : "failed");

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
