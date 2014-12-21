#include "socketworker.h"
#include <QDebug>

SocketWorker::SocketWorker(QTcpSocket *socket, QString host_, quint16 port_, QObject *parent) :
    QObject(parent)
{
    host = host_;
    port = port_;
    currentConnection = socket;
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
        while(*packetSize == 0&&dataBuffer->size()>=sizeBlock||(*packetSize>0&&dataBuffer->size()>=*packetSize)){
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
