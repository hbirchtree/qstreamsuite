#ifndef SOCKETWORKER_H
#define SOCKETWORKER_H

#include <QThread>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDataStream>
#include <QByteArray>
#include <QHostAddress>
#include <QEventLoop>

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    SocketWorker(QTcpSocket *socket, QString host_, quint16 port_, QObject *parent = 0); //Create a socket
    SocketWorker(QTcpSocket *socket,QObject *parent = 0); //Use an existing connection
    ~SocketWorker();
    void start();

signals:
    void dataReceived(QByteArray *data);
    void clientConnected();
    void clientDisconnected();
    void reportLogEntry(QString);
    void forwardError(QString);
    void clientFailed();
public slots:
    void transmitData(QByteArray *data);
    void handleClientDisconnect();

private slots:
    void receiveData();
    void handleSocketError(QAbstractSocket::SocketError error);
    void clientDisconnect();

private:
    QTcpSocket *currentConnection;
    QByteArray *dataBuffer;
    quint32* packetSize;
    int sizeBlock;

    QString host;
    quint16 port;

};

#endif // SOCKETWORKER_H
