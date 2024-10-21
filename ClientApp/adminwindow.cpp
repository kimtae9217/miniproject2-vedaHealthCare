#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "chatwindow.h"
#include <QLabel>
#include <QApplication>
#include <QMessageBox>

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

    connect(ui->customerList, &QListWidget::itemClicked, this, &AdminWindow::on_customerList_itemClicked);
    connect(ui->chatConnectButton, &QPushButton::clicked, this, &AdminWindow::on_chatConnectButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &AdminWindow::on_exitButton_clicked);

    // 선택된 고객 이름을 저장하기 위한 변수
    selectedCustomer = "";
}


AdminWindow::~AdminWindow()
{
    delete ui;
}

// 고객 이름을 클릭했을 때 채팅 내역을 로드하는 함수
void AdminWindow::on_customerList_itemClicked(QListWidgetItem *item)
{
    QString customerName = item->text();
    selectedCustomer = item->text();  // 선택된 고객 이름 저장
    loadChatHistory(customerName);
}


// 채팅 내역을 로드하는 함수 (예시)
void AdminWindow::loadChatHistory(const QString& customerName)
{
    ui->chatHistory->clear(); // 기존 채팅 내역 삭제
    ui->chatHistory->append("Chat with " + customerName + ": Hi, how can I help?");
    ui->chatHistory->append("You: I need assistance with my account.");


    // QTextBrowser에 고객 정보를 출력
    ui->chatHistory->clear();  // 기존 텍스트 삭제
    ui->chatHistory->append("Customer Information:");
    ui->chatHistory->append("Name: " + customerName);
    ui->chatHistory->append("Age: 30");  // 예시 데이터
    ui->chatHistory->append("Email: customer@example.com");  // 예시 데이터
    ui->chatHistory->append("Phone: 010-1234-5678");  // 예시 데이터
}

// "채팅 접속" 버튼이 눌렸을 때 새로운 채팅 창을 띄움
void AdminWindow::on_chatConnectButton_clicked()
{
    if (selectedCustomer.isEmpty()) {
        QMessageBox::warning(this, "Error", "고객을 먼저 선택하세요!");  // 고객 선택 안된 경우 경고
        return;
    }

    // 새 채팅 창을 띄우고 고객과의 채팅 시작
    ChatWindow *chatWindow = new ChatWindow(this, selectedCustomer);  // 고객 이름 전달
    chatWindow->show();
}

// 종료 버튼 클릭 시 호출되는 슬롯
void AdminWindow::on_exitButton_clicked()
{
    QApplication::quit();  // 프로그램 종료
}
