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
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_customerList_itemClicked(QListWidgetItem *item); // 고객 이름 클릭
    void on_customerList_itemDoubleClicked(QListWidgetItem *item); // 고객 이름 더블 클릭

private:
    Ui::AdminWindow *ui;
    void loadChatHistory(const QString& customerName);  // 채팅 내역 로드
    void showCustomerInfo(const QString& customerName); // 고객 정보 표시
};

#endif // ADMINWINDOW_H
