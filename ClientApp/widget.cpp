#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , loginWidget(nullptr)
{
    ui->setupUi(this);

    // UI 요소 연결
    idLineEdit = findChild<QLineEdit*>("lineEdit");
    pwLineEdit = findChild<QLineEdit*>("lineEdit_2");
    loginButton = findChild<QPushButton*>("pushButton");
    signInButton = findChild<QPushButton*>("pushButton_2");

    // 비밀번호 입력 시 '*'로 표시
    pwLineEdit->setEchoMode(QLineEdit::Password);

    // 버튼 클릭 시 해당 함수 호출
    connect(loginButton, &QPushButton::clicked, this, &Widget::onLoginClicked);
    connect(signInButton, &QPushButton::clicked, this, &Widget::onSignInClicked);
}

Widget::~Widget()
{
    delete ui;
    if (loginWidget)
        delete loginWidget;
}

void Widget::onLoginClicked()
{
    QString id = idLineEdit->text();
    QString password = pwLineEdit->text();

    if (validateLogin(id, password)) {
        QMessageBox::information(this, "로그인 성공", "환영합니다, " + id + "님!");
        // TODO: 로그인 성공 후 메인 화면으로 전환하는 코드 추가하기
    } else {
        QMessageBox::warning(this, "로그인 실패", "아이디 또는 비밀번호가 올바르지 않습니다.");
    }
}

void Widget::onSignInClicked()
{
    if (!loginWidget)
        loginWidget = new login();

    loginWidget->show();
    this->hide();
}

bool Widget::validateLogin(const QString &id, const QString &password)
{
    // TODO: 실제 데이터베이스나 서버와의 통신을 통한 로그인 검증 로직으로 대체
    return (id == "user" && password == "password");
}
