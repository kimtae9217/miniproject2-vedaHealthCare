#include "widget.h"
#include <QtWidgets>
#include <QtNetwork>

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
    updateButtonStates();
}

void Widget::clientConnect()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);
    statusLabel->setText("New client connected");
}

void Widget::updateButtonStates()
{
    startButton->setEnabled(tcpServer == nullptr);
    stopButton->setEnabled(tcpServer != nullptr);
}
