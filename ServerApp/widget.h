#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

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
    void sendServerInfo();

private:
    QLabel *statusLabel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clientSockets;
    void updateButtonStates();
    QString getLocalIPAddress();
};

#endif // WIDGET_H
