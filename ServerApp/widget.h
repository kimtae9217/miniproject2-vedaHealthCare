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
    bool registerUser(const QString &name, const QString &age, const QString &email, const QString &userid, const QString &password);
    void processClientData(QTcpSocket *clientSocket);
    void sendMessageToClient(const QString& customerName, const QString& message);

private:
    QLabel *statusLabel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTcpServer *tcpServer;
    // QList<QTcpSocket*> clientSockets;
    void updateButtonStates();
    QString getLocalIPAddress();
    QSqlDatabase db;
    QMap<QString, QTcpSocket*> clientSockets;  // 고객 이름과 소켓을 매핑
};

#endif // WIDGET_H
