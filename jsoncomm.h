#ifndef JSONCOMM_H
#define JSONCOMM_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QByteArray>
#include <QHash>
#include <QJsonValue>
#include <QVariant>
#include <QPointer>
#include "streamer_enums.h"

class JsonComm : public QObject
{
    Q_OBJECT
public:
    explicit JsonComm(QObject *parent = 0);
//    QByteArray *createTransmission(int type,const QString &data); //Universal
    QByteArray *createTransmission(qint8 destination, QByteArray data); //Data
    QByteArray *createTransmission(qint8 destination, QString data);
    QByteArray *createTransmission(qint16 command, qint32 value); //Command
    QByteArray *createTransmission(qint16 type,qint64 value1,qint64 value2); //Input

public slots:
    void interpretTransmission(QByteArray *transmission);

signals:
    void newInputSignal(qint16 type, qint64 value1, qint64 value2); //Used for both keys and 2D points, thus there are two values possible
    void newStringSignal(QString payload);
    void newDataSignal(qint8 route,QByteArray *data);
    void newCommandSignal(qint8 c,qint16 v);
    void reportLogEntry(QString message);
};

#endif // JSONCOMM_H
