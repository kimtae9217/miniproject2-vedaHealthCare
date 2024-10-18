#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , loginWidget(nullptr)
    , mainWindow(nullptr)
    , adminWindow(nullptr)
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
    connect(loginWidget, &login::backButtonClicked, this, &Widget::onLoginBackButtonClicked);
}

Widget::~Widget()
{
    delete ui;
    if (loginWidget)
        delete loginWidget;
    if (mainWindow)
        delete mainWindow;
    if (adminWindow)
        delete adminWindow;
}

void Widget::onLoginClicked()
{
    QString id = idLineEdit->text();
    QString password = pwLineEdit->text();

    // if (validateLogin(id, password)) {
    //     QMessageBox::information(this, "로그인 성공", "환영합니다, " + id + "님!");
    //     // TODO: 로그인 성공 후 메인 화면으로 전환하는 코드 추가하기
    // } else {
    //     QMessageBox::warning(this, "로그인 실패", "아이디 또는 비밀번호가 올바르지 않습니다.");
    // }

    // 임시로 모든 로그인 시도를 성공으로 처리
    QMessageBox::information(this, "로그인 성공", "환영합니다, " + id + "님!");


    // MainWindow로 전환
    if (id == "123" && password == "123") {
        adminWindow = new AdminWindow();
        adminWindow->show();
    } else {
        mainWindow = new MainWindow();
        mainWindow->show();
    }
    this->hide(); // 현재 로그인 창 숨기기
}

void Widget::onSignInClicked()
{
    if (!loginWidget){
        loginWidget = new login();
        connect(loginWidget, &login::backButtonClicked, this, &Widget::onLoginBackButtonClicked);

    }
    loginWidget->show();
    this->hide();
}

void Widget::onLoginBackButtonClicked()
{
    // 로그인 위젯 숨기기
    loginWidget->hide();

    // 메인 위젯(현재 클래스) 보이기
    this->show();
}

// bool Widget::validateLogin(const QString &id, const QString &password)
// {
//     // TODO: 실제 데이터베이스나 서버와의 통신을 통한 로그인 검증 로직으로 대체
//     return (id == "user" && password == "password");
// }
