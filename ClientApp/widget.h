#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "login.h"
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onLoginClicked();
    void onSignInClicked();

private:
    Ui::Widget *ui;
    QLineEdit *idLineEdit;
    QLineEdit *pwLineEdit;
    QPushButton *loginButton;
    QPushButton *signInButton;
    login *loginWidget;
    MainWindow *mainWindow;

    bool validateLogin(const QString &id, const QString &password);
};
#endif // WIDGET_H
