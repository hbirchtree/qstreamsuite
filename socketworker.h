#ifndef SOCKETWORKER_H
#define SOCKETWORKER_H

#include <QThread>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDataStream>
#include <QByteArray>
#include <QHostAddress>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "streamer_enums.h"

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    SocketWorker(QTcpSocket *socket, QString host_, quint16 port_, QObject *parent = 0); //Create a socket
    SocketWorker(QTcpSocket *socket,QObject *parent = 0); //Use an existing connection
    ~SocketWorker();
    void start();

    QByteArray *createTransmission(qint8 destination, QByteArray data); //Data
    QByteArray *createTransmission(qint8 destination, QString data);
    QByteArray *createTransmission(qint16 command, qint64 value); //Command
    QByteArray *createTransmission(qint16 type,qint64 value1,qint64 value2); //Input

signals:
    void dataReceived(QByteArray *data);
    void clientConnected();
    void clientConnecting();
    void clientDisconnected();
    void reportLogEntry(QString);
    void forwardError(QString);
    void clientFailed();

    void newInputSignal(qint16 type, qint64 value1, qint64 value2); //Used for both keys and 2D points, thus there are two values possible
    void newStringSignal(QString payload);
    void newDataSignal(qint8 route,QByteArray *data);
    void newCommandSignal(qint8 c,qint64 v);

public slots:
    void transmitData(QByteArray *data);
    void handleClientDisconnect();

    void interpretTransmission(QByteArray *transmission);

    void sendPacket(qint8 destination, QByteArray *data); //data
    void sendPacket(qint8 destination, QString data); //stringsignal
    void sendPacket(qint16 command, qint64 value); //command
    void sendPacket(qint16 type, qint64 value1, qint64 value2); //data

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
