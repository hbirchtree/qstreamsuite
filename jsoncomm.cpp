#include "jsoncomm.h"
#include <QDebug>

JsonComm::JsonComm(QObject *parent) :
    QObject(parent)
{

}

//QByteArray* JsonComm::createTransmission(int type,const QString &data){
//    QJsonDocument container;
//    QJsonObject transmission;
//    QByteArray *tdata;
//    transmission.insert("t",type);
//    switch(type){
//    case StreamerEnums::TRANSMISSION_INPUT:
//        transmission.insert("i",data); break;
//    case StreamerEnums::TRANSMISSION_DATA:
//        transmission.insert("d",data); break;
//    case StreamerEnums::TRANSMISSION_COMMAND:
//        transmission.insert("c",data); break;
//    }
//    container.setObject(transmission);
//    tdata = new QByteArray(container.toJson());
//    return tdata;
//}

QByteArray* JsonComm::createTransmission(qint16 type,qint64 value1,qint64 value2){
    QJsonDocument container;
    QJsonObject transmission;
    QByteArray *tdata;
    transmission.insert("t",StreamerEnums::TRANSMISSION_INPUT);
    transmission.insert("i",QString::number(type)+":"+QString::number(value1)+","+QString::number(value2));
    container.setObject(transmission);
    tdata = new QByteArray(container.toJson());
    return tdata;
}

QByteArray* JsonComm::createTransmission(qint8 destination, QByteArray data){
    QJsonDocument container;
    QJsonObject transmission;
    QByteArray *tdata;
    transmission.insert("t",StreamerEnums::TRANSMISSION_DATA);
    transmission.insert("s",destination);
    transmission.insert("d",QString(data.toBase64()));
    container.setObject(transmission);
    tdata = new QByteArray(container.toJson());
    return tdata;
}

QByteArray* JsonComm::createTransmission(qint8 destination, QString data){
    QJsonDocument container;
    QJsonObject transmission;
    QByteArray *tdata;
    transmission.insert("t",StreamerEnums::TRANSMISSION_DATA);
    transmission.insert("s",destination);
    transmission.insert("d",data);
    container.setObject(transmission);
    tdata = new QByteArray(container.toJson());
    return tdata;
}

QByteArray* JsonComm::createTransmission(qint16 command, qint32 value){
    QJsonDocument container;
    QJsonObject transmission;
    QByteArray *tdata;
    transmission.insert("t",StreamerEnums::TRANSMISSION_COMMAND);
    transmission.insert("c",QString::number(command)+":"+QString::number(value));
    container.setObject(transmission);
    tdata = new QByteArray(container.toJson());
    return tdata;
}

void JsonComm::interpretTransmission(QByteArray *transmission){
    QJsonParseError *jerror = new QJsonParseError;
    QJsonDocument *payloadD = new QJsonDocument;
    *payloadD = payloadD->fromJson(*transmission,jerror);
    if(jerror->error != QJsonParseError::NoError){
        reportLogEntry("JSON: "+jerror->errorString());
        return;
    }
    QJsonObject *payload = new QJsonObject(payloadD->object());
    delete jerror; delete payloadD;
    switch(payload->value("t").toInt(0)){
    case StreamerEnums::TRANSMISSION_DATA:{
        QByteArray *data = new QByteArray(payload->value("d").toString().toLocal8Bit());
        qint8 slot = payload->value("s").toInt();
        newDataSignal(slot,data); break;
    }
    case StreamerEnums::TRANSMISSION_INPUT:{
        QStringList *input = new QStringList(payload->value("i").toString().split(":"));
        QStringList *v = new QStringList(input->at(1).split(","));
        qint16 t = (qint16)input->at(0).toInt();
        qint64 v1 = (qint64)v->at(0).toInt();
        qint64 v2 = 0;
        if(v->size()>1) //There must be a better way of doing this.
            v2 = (qint64)v->at(1).toInt();
        delete input;
        newInputSignal(t,v1,v2);
        break;
    }
    case StreamerEnums::TRANSMISSION_COMMAND:{
        QStringList *input = new QStringList(payload->value("c").toString().split(":"));
        qint8 c = (qint8)input->at(0).toInt();
        qint16 v = (qint16)input->at(1).toInt();
        emit newCommandSignal(c,v);break;
    }
    default:
        reportLogEntry("JSON: "+tr("Unable to determine transmission type"));
        qDebug() << transmission->toBase64();
    }
    delete payload;
}
