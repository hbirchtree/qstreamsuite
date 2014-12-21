#ifndef NETWORKER_H
#define NETWORKER_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include "../socketworker.h"

class NetWorker : public QThread
{
    Q_OBJECT
public:
    explicit NetWorker(QObject *parent = 0);
    ~NetWorker();
    int setupServer(quint16 port, QString *interfaceString);

signals:
    void reportStatusText(QString);
    void requestUserSelectFromList(QString,QStringList,QString*);
    void reportLogEntry(QString);
    void clientConnected();
    void emitSocket(SocketWorker*);

private slots:
    void handleClient();
    void handleClientDisconnect();
    void handleSocketError(QAbstractSocket::SocketError error);

private:
    QTcpServer *tcpServer;
    QList<SocketWorker*> *connections;
//    SocketWorker *currentConnection;

    QNetworkInterface *selectNetInterface();
    bool activeConnection;
};

#endif // NETWORKER_H
