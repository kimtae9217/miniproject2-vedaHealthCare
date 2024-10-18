#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QFormLayout>
#include <QLabel>

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

    // 시작할 때 고객 정보 위젯과 채팅 기록, Close 버튼 숨기기
    ui->customerInfoWidget->setVisible(false);  // 고객 정보 부분 숨김
    ui->closeButton->setVisible(false);  // Close 버튼 숨김

    // 예시 고객 리스트 추가
    ui->customerList->addItem("Customer A");
    ui->customerList->addItem("Customer B");
    ui->customerList->addItem("Customer C");

    connect(ui->customerList, &QListWidget::itemClicked, this, &AdminWindow::on_customerList_itemClicked);
    connect(ui->customerList, &QListWidget::itemDoubleClicked, this, &AdminWindow::on_customerList_itemDoubleClicked);
}


AdminWindow::~AdminWindow()
{
    delete ui;
}

// 고객 이름을 클릭했을 때 채팅 내역을 로드하는 함수
void AdminWindow::on_customerList_itemClicked(QListWidgetItem *item)
{
    QString customerName = item->text();
    loadChatHistory(customerName);
}

// 고객 이름을 두 번 클릭했을 때 고객 정보를 표시하는 함수
void AdminWindow::on_customerList_itemDoubleClicked(QListWidgetItem *item)
{
    QString customerName = item->text();
    showCustomerInfo(customerName);
}

// 채팅 내역을 로드하는 함수 (예시)
void AdminWindow::loadChatHistory(const QString& customerName)
{
    ui->chatHistory->clear(); // 기존 채팅 내역 삭제
    ui->chatHistory->append("Chat with " + customerName + ": Hi, how can I help?");
    ui->chatHistory->append("You: I need assistance with my account.");
}

// 고객 정보를 표시하는 함수 (QFormLayout 사용)
void AdminWindow::showCustomerInfo(const QString& customerName)
{
    // 기존 레이아웃이 있으면 제거하여 모든 정보를 초기화
    if (ui->customerInfoWidget->layout() != nullptr) {
        QLayout *oldLayout = ui->customerInfoWidget->layout();  // 기존 레이아웃 저장
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {  // 기존 레이아웃에서 위젯 제거
            delete item->widget();  // 각 위젯 삭제
            delete item;  // 레이아웃 아이템 삭제
        }
        delete oldLayout;  // 레이아웃 자체 삭제
    }

    // 고객 정보가 표시될 새로운 FormLayout 설정
    QFormLayout *formLayout = new QFormLayout(ui->customerInfoWidget);

    // 고객 정보 추가
    QLabel *nameLabel = new QLabel(customerName);
    QLabel *ageLabel = new QLabel("30");  // 예시 데이터
    QLabel *emailLabel = new QLabel("customer@example.com");  // 예시 데이터
    QLabel *phoneLabel = new QLabel("010-1234-5678");  // 예시 데이터

    formLayout->addRow("Name:", nameLabel);
    formLayout->addRow("Age:", ageLabel);
    formLayout->addRow("Email:", emailLabel);
    formLayout->addRow("Phone:", phoneLabel);

    // Close 버튼은 기존에 추가된 버튼을 보이게 설정 (새로 생성하지 않음)
    ui->closeButton->setVisible(true);  // Close 버튼 보이게 설정
    connect(ui->closeButton, &QPushButton::clicked, [=]() {
        ui->customerInfoWidget->setVisible(false);  // 고객 정보 위젯 숨기기
        ui->closeButton->setVisible(false);  // Close 버튼 숨기기
    });

    // 새 레이아웃 설정
    ui->customerInfoWidget->setLayout(formLayout);

    // 고객 정보 위젯을 보이게 설정
    ui->customerInfoWidget->setVisible(true);
}
