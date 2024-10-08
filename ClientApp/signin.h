#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

signals:
    void backButtonClicked();

private slots:
    void onRegisterClicked();
    void onBackButtonClicked();

private:
    Ui::login *ui;
};

#endif // SIGNIN_H
