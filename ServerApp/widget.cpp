#include "widget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtNetwork>
#include <QDebug>

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
    connect(clientSocket, &QAbstractSocket::disconnected,clientSocket, &QObject::deleteLater);
    connect(clientSocket, &QAbstractSocket::disconnected,
            [this, clientSocket]() { clientSockets.removeOne(clientSocket); });

    clientSockets.append(clientSocket);
    statusLabel->setText("New client connected");
    qDebug() << "New client connected";

    sendServerInfo();
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
