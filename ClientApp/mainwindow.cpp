#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (socket) {
        socket->close();
        delete socket;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        ui->pushButton->setText("채팅 접속");
        ui->listWidget->addItem("서버와의 연결이 끊어졌습니다.");
    } else {
        connectToServer();
    }
}

void MainWindow::connectToServer()
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, [this]() {
        ui->listWidget->addItem("서버에 연결되었습니다.");
        ui->pushButton->setText("연결 끊기");
    });

    connect(socket, &QTcpSocket::disconnected, [this]() {
        ui->listWidget->addItem("서버와의 연결이 끊어졌습니다.");
        ui->pushButton->setText("채팅 접속");
    });

    connect(socket, &QTcpSocket::errorOccurred, [this](QAbstractSocket::SocketError socketError) {
        ui->listWidget->addItem("연결 오류: " + socket->errorString());
    });

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);

    socket->connectToHost("127.0.0.1", 8080);  // localhost의 8080 포트로 연결
}

void MainWindow::onReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_0);

    if (socket->bytesAvailable() < static_cast<qint64>(sizeof(quint16))) {
        return;
    }

    QString messageType;
    in >> messageType;

    if (messageType == "SERVER_INFO") {
        QString serverIP;
        quint16 serverPort;
        in >> serverIP >> serverPort;
        ui->listWidget->addItem(QString("서버 정보: IP %1, Port %2").arg(serverIP).arg(serverPort));
    }
}
