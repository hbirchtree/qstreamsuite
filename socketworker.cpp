#include "socketworker.h"
#include <QDebug>

SocketWorker::SocketWorker(QTcpSocket *socket, QString host_, quint16 port_, QObject *parent) :
    QObject(parent)
{
    host = host_;
    port = port_;
    currentConnection = socket;
//    connect(this,SIGNAL(dataReceived(QByteArray*)),SLOT(interpretTransmission(QByteArray*)));
}

SocketWorker::SocketWorker(QTcpSocket *socket, QObject *parent) : QObject(parent){
    currentConnection = socket;
}

SocketWorker::~SocketWorker(){
    delete dataBuffer;
    delete packetSize;
}

void SocketWorker::start(){
    sizeBlock = (int)sizeof(quint32);
    dataBuffer = new QByteArray();
    packetSize = new quint32(0);
    connect(currentConnection,SIGNAL(readyRead()),SLOT(receiveData()));
    connect(currentConnection,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(handleSocketError(QAbstractSocket::SocketError)));
    connect(currentConnection,SIGNAL(aboutToClose()),SLOT(handleClientDisconnect()));
    connect(currentConnection,SIGNAL(disconnected()),SLOT(clientDisconnect())); //For when the *client* disconnects from the server or vice versa

    if(!currentConnection->open(QIODevice::ReadWrite)){
        reportLogEntry(tr("TCP: Failed to open TCP socket to client as R/W"));
    }
    if(currentConnection->state()!=QAbstractSocket::ConnectedState){
        currentConnection->connectToHost(QHostAddress(host),port);
        if(!currentConnection->waitForConnected(5000)){
//            reportLogEntry(tr("TCP: Client failed to connect"));
            emit clientFailed();
            return;
        }
    }
    reportLogEntry(tr("TCP: Client connected successfully"));
    emit clientConnected();
}

void SocketWorker::receiveData(){
    while(currentConnection->bytesAvailable()>0){
        dataBuffer->append(currentConnection->readAll());
        while((*packetSize == 0&&dataBuffer->size()>=sizeBlock)||(*packetSize>0&&dataBuffer->size()>=*packetSize)){
            if(dataBuffer->size() >= sizeBlock&&*packetSize==0){
                QByteArray s_buffer(dataBuffer->mid(0,sizeBlock));
                QDataStream s_stream(&s_buffer,QIODevice::ReadOnly);
                s_stream >> *packetSize;
                *dataBuffer = dataBuffer->remove(0,sizeBlock);
                if(*packetSize == 0)
                    reportLogEntry("TCP: Invalid block size encountered, transmission ignored");
            }
            if(*packetSize > 0 && dataBuffer->size() >= *packetSize){
                QByteArray *transmission = new QByteArray(dataBuffer->mid(0,*packetSize));
                *dataBuffer = dataBuffer->remove(0,*packetSize);
                *packetSize = 0;
                emit dataReceived(transmission);
            }
        }
    }
}

void SocketWorker::transmitData(QByteArray *data){
    if(currentConnection->state()!=QAbstractSocket::ConnectedState){
        reportLogEntry(tr("TCP: Tried to transmit data but socket was not connected"));
        return;
    }
    quint32 s = (quint32)(data->size());
    QByteArray s_buffer;
    QDataStream s_stream(&s_buffer,QIODevice::WriteOnly);
    s_stream << s;
    if(currentConnection->write(s_buffer)<0){
        reportLogEntry(tr("TCP: Failed to transfer size"));
        qDebug() << s_buffer;
        reportLogEntry(currentConnection->errorString());
    }
    if(currentConnection->write(*data)<0){
        reportLogEntry(tr("TCP: Failed to transfer data"));
        qDebug() << data->toBase64();
        reportLogEntry(currentConnection->errorString());
    }
    if(!currentConnection->waitForBytesWritten())
        reportLogEntry("TCP: Failed to write all bytes");
}

void SocketWorker::handleSocketError(QAbstractSocket::SocketError error){
    switch(error){
    case QAbstractSocket::RemoteHostClosedError:
        return;
    default:
        reportLogEntry(tr("Error ocurred: %1").arg(currentConnection->errorString()));
        emit clientFailed();
    }
}

void SocketWorker::handleClientDisconnect(){
    if(currentConnection->state()==QAbstractSocket::ConnectedState){
        currentConnection->disconnectFromHost();
    }
}

void SocketWorker::clientDisconnect(){
    reportLogEntry(tr("TCP: Client disconnected from server"));
    emit clientDisconnected();
}

QByteArray* SocketWorker::createTransmission(qint16 type,qint64 value1,qint64 value2){
    QJsonDocument container;
    QJsonObject transmission;
    QByteArray *tdata;
    transmission.insert("t",StreamerEnums::TRANSMISSION_INPUT);
    transmission.insert("i",QString::number(type)+":"+QString::number(value1)+","+QString::number(value2));
    container.setObject(transmission);
    tdata = new QByteArray(container.toJson());
    return tdata;
}

QByteArray* SocketWorker::createTransmission(qint8 destination, QByteArray data){
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

QByteArray* SocketWorker::createTransmission(qint8 destination, QString data){
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

QByteArray* SocketWorker::createTransmission(qint16 command, qint64 value){
    QJsonDocument container;
    QJsonObject transmission;
    QByteArray *tdata;
    transmission.insert("t",StreamerEnums::TRANSMISSION_COMMAND);
    transmission.insert("c",QString::number(command)+":"+QString::number(value));
    container.setObject(transmission);
    tdata = new QByteArray(container.toJson());
    return tdata;
}

void SocketWorker::interpretTransmission(QByteArray *transmission){
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
        qint64 v = (qint64)input->at(1).toInt();
        emit newCommandSignal(c,v);break;
    }
    default:
        reportLogEntry("JSON: "+tr("Unable to determine transmission type"));
        qDebug() << transmission->toBase64();
    }
    delete payload;
}

void SocketWorker::sendPacket(qint8 destination, QByteArray *data){
    transmitData(createTransmission(destination,*data));
}

void SocketWorker::sendPacket(qint8 destination, QString data){
    transmitData(createTransmission(destination,data));
}

void SocketWorker::sendPacket(qint16 command, qint64 value){
    transmitData(createTransmission(command,value));
}

void SocketWorker::sendPacket(qint16 type, qint64 value1, qint64 value2){
    transmitData(createTransmission(type,value1,value2));
}
