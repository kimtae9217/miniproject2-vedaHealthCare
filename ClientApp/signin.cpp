#include "signin.h"
#include "ui_signin.h"
#include <QMessageBox>
#include <QPushButton>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    connect(ui->signInButton, &QPushButton::clicked, this, &login::onRegisterClicked);

    connect(ui->backButton, &QPushButton::clicked, this, &login::onBackButtonClicked);

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

    // TODO: 실제 회원가입 로직 구현 필요 (데이터베이스에 저장 등)

    QMessageBox::information(this, "회원가입 성공", "회원가입이 완료되었습니다.");
    this->close();
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
