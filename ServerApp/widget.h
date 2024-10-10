#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>

class QLabel;
class QPushButton;

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);

private slots:
    void startServer();
    void stopServer();
    void clientConnect();

private:
    QLabel *statusLabel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTcpServer *tcpServer;
    void updateButtonStates();
};

#endif // WIDGET_H
