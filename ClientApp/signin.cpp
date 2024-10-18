#include "signin.h"
#include "ui_signin.h"
#include <QMessageBox>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
    , tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(ui->signInButton, &QPushButton::clicked, this, &login::onRegisterClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &login::onBackButtonClicked);

    // 서버 연결 설정
    tcpSocket->connectToHost("localhost", 8080); // 서버 IP와 포트를 적절히 수정하세요
    connect(tcpSocket, &QTcpSocket::connected, this, &login::onConnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &login::onReadyRead);

}

login::~login()
{
    delete ui;
}

void login::onRegisterClicked()
{
    // 입력값 가져오기
    QString email = ui->lineEdit->text();
    QString id = ui->lineEdit_2->text();
    QString password = ui->lineEdit_3->text();
    QString confirmPassword = ui->lineEdit_4->text();

    // 유효성 검사
    if (email.isEmpty() || id.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "모든 필드를 입력해주세요.");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "비밀번호 오류", "비밀번호가 일치하지 않습니다.");
        return;
    }

    // 서버로 데이터 전송
    QJsonObject json;
    json["type"] = "REGISTER";
    json["email"] = email;
    json["id"] = id;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    tcpSocket->write(data);
    //QMessageBox::information(this, "회원가입 성공", "회원가입이 완료되었습니다.");
    //this->close();
}

void login::onBackButtonClicked()
{
    // 입력 필드 초기화
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();

    // 뒤로가기 시그널 발생
    emit backButtonClicked();

    this->hide();
}

void login::onConnected()
{
    qDebug() << "Connected to server";
}

void login::onReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if (json["type"].toString() == "REGISTER_RESPONSE") {
        bool success = json["success"].toBool();
        if (success) {
            QMessageBox::information(this, "회원가입 성공", "회원가입이 완료되었습니다.");
            emit registrationSuccessful();  // 성공 시그널 발생
            this->hide();
        } else {
            QMessageBox::warning(this, "회원가입 실패", "회원가입에 실패했습니다. 다시 시도해주세요.");
        }
    }
}
