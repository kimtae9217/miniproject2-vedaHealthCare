#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

class QPushButton;
namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_customerList_itemClicked(QListWidgetItem *item); // 고객 이름 클릭
    void on_chatConnectButton_clicked();
    void on_exitButton_clicked();  // 종료 버튼 클릭 시 호출될 슬롯


private:
    Ui::AdminWindow *ui;
    void loadChatHistory(const QString& customerName);  // 채팅 내역 로드
    QString selectedCustomer;

};

#endif // ADMINWINDOW_H
