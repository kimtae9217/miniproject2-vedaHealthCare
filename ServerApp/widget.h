#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtSql>

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
    bool initDatabase();
    bool registerUser(const QString &email, const QString &username, const QString &password);
    void processClientData(QTcpSocket *clientSocket);

private:
    QLabel *statusLabel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clientSockets;
    void updateButtonStates();
    QString getLocalIPAddress();
    QSqlDatabase db;
};

#endif // WIDGET_H
