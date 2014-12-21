#include "networker.h"
#include <QDebug>

NetWorker::NetWorker(QObject *parent) :
    QThread(parent)
{
    activeConnection = false;
    tcpServer = new QTcpServer(this);
    connections = new QList<SocketWorker*>();
}

NetWorker::~NetWorker(){
    delete tcpServer;
}

int NetWorker::setupServer(quint16 port, QString *interfaceString){
    QNetworkInterface *currentInterface;
    if(!interfaceString->isEmpty()){
        currentInterface = new QNetworkInterface(QNetworkInterface::interfaceFromName(*interfaceString));
    }else
         currentInterface = selectNetInterface();
    QHostAddress listenAddress;
    foreach(QNetworkAddressEntry e,currentInterface->addressEntries())
        if(e.ip()!=QHostAddress::AnyIPv6){
            listenAddress = e.ip();
            break;
        }
    if(!tcpServer->listen(listenAddress,port)){
        emit reportStatusText(QString("TCP error: "+tcpServer->errorString()));
        return 1;
    }
    reportStatusText(tr("Selected %1 as the main interface,\nbound to port %2 with address %3.").arg(currentInterface->humanReadableName(),QString::number(tcpServer->serverPort()),tcpServer->serverAddress().toString()));
    connect(tcpServer,SIGNAL(newConnection()),SLOT(handleClient()));
    connect(tcpServer,SIGNAL(acceptError(QAbstractSocket::SocketError)),SLOT(handleSocketError(QAbstractSocket::SocketError)));
    return 0;
}

QNetworkInterface* NetWorker::selectNetInterface(){
    QList<QNetworkInterface> *interfaces = new QList<QNetworkInterface>(QNetworkInterface::allInterfaces());
    QStringList interfaceStrings;
    foreach(QNetworkInterface intf,*interfaces){
        interfaceStrings.append(intf.humanReadableName());
    }
    delete interfaces;
    QString *targetInterface = new QString();
    requestUserSelectFromList(tr("Select the interface you want to use:"),interfaceStrings,targetInterface);
    QNetworkInterface *selectedInterface = new QNetworkInterface(QNetworkInterface::interfaceFromName(*targetInterface));
    delete targetInterface;
    return selectedInterface;
}

void NetWorker::handleClient(){
    if(activeConnection){
        reportLogEntry(tr("TCP: Client attempted to connect while the server is already occupied"));
        tcpServer->nextPendingConnection()->disconnectFromHost();
        return;
    }
    activeConnection = true;
    SocketWorker *currentConnection = new SocketWorker(tcpServer->nextPendingConnection(),this);
    connect(currentConnection,SIGNAL(clientDisconnected()),SLOT(handleClientDisconnect()));
    connections->append(currentConnection);
    emit emitSocket(currentConnection);
}

void NetWorker::handleSocketError(QAbstractSocket::SocketError error){
    switch(error){
    case QAbstractSocket::RemoteHostClosedError:
        return;
    default:
        reportLogEntry(tr("Error ocurred: %1").arg(tcpServer->errorString()));
    }
}

void NetWorker::handleClientDisconnect(){
    activeConnection = false;
}
