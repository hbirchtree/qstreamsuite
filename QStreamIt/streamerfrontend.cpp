#include "streamerfrontend.h"
#include <QDebug>

StreamerFrontend::StreamerFrontend(QWidget *parent) :
    QWidget(parent)
{

    dataStore = new DataStore(this);
    dataStore->start();
    g_connectedState = false;
    setupQMLUI(QUrl("qrc:/qmlui/Display.qml"));
}

StreamerFrontend::~StreamerFrontend()
{
    foreach(InputPluginInterface* interf,inputStack){
        delete interf;
    }
}

void StreamerFrontend::setupInputSocket(QString ipAddress, quint16 port){
    inputPipeHandler = new JsonComm(this);
    QTcpSocket *socket = new QTcpSocket(this);
    inputPipe = new SocketWorker(socket,ipAddress,port,this);

    connect(inputPipe,SIGNAL(forwardError(QString)),SLOT(displayError(QString)));
    connect(inputPipe,SIGNAL(reportLogEntry(QString)),SLOT(displayError(QString)));
    connect(inputPipe,SIGNAL(clientDisconnected()),SLOT(socket_disconnectServer()));
    connect(inputPipe,SIGNAL(clientFailed()),SLOT(socket_disconnectServer()));
    connect(inputPipe,SIGNAL(clientFailed()),qmlConnector,SLOT(fail_disconnect()));
    connect(inputPipeHandler,SIGNAL(newCommandSignal(qint8,qint16)),SLOT(handleCommandSignal(qint8,qint16)));
    connect(inputPipeHandler,SIGNAL(newDataSignal(qint8,QByteArray*)),dataStore,SLOT(set_data(qint8,QByteArray*)));
    connect(this,SIGNAL(requestData(qint8,QByteArray*,bool*)),dataStore,SLOT(get_data(qint8,QByteArray*,bool*)));
//    connect(inputPipeHandler,SIGNAL(newDataSignal(qint16,QByteArray*)),SLOT(printPack(qint16,QByteArray*)));
    connect(inputPipeHandler,SIGNAL(reportLogEntry(QString)),SLOT(displayError(QString)));
    connect(inputPipe,SIGNAL(dataReceived(QByteArray*)),inputPipeHandler,SLOT(interpretTransmission(QByteArray*)));
    connect(inputPipe,SIGNAL(clientConnected()),qmlConnector,SLOT(connectedServer()));

    connect(inputPipe,SIGNAL(destroyed()),inputPipeHandler,SLOT(deleteLater()));
//    connect(inputPipe,SIGNAL(clientDisconnected()),inputPipe,SLOT(deleteLater()));
    connect(inputPipe,SIGNAL(clientFailed()),inputPipe,SLOT(deleteLater()));

    inputPipe->start();
    g_connectedState = true;
}

void StreamerFrontend::setupQMLUI(QUrl qmlUi){
    qmlBase = new QQmlApplicationEngine(this);
    qmlRegisterType<EventCapture>("EventCapture",1,0,"EventCaptor");
    qmlRegisterType<AnalogStickSurface>("AnalogStickComponent",1,0,"AnalogStick");
    qmlRegisterType<TrackPointComponent>("TrackPointComponent",1,0,"TrackPoint");
    qmlBase->load(qmlUi);
    qmlConnector = qmlBase->rootObjects().at(0);

    connect(this,SIGNAL(qmlConnectedStatus()),qmlConnector,SLOT(connectedServer()));
    connect(qmlConnector,SIGNAL(captureEvent(int,int,int)),SLOT(sendEvent(int,int,int)));
    connect(this,SIGNAL(qmlDisconnectedStatus()),qmlConnector,SLOT(main_disconnect()));
    connect(this,SIGNAL(updateStatusText(QVariant)),qmlConnector,SLOT(statusBubbleShow(QVariant)));
//    connect(qmlConnector,SIGNAL(captureEvent(int,int,int)),SLOT(printPack(int,int,int)));
    connect(qmlConnector,SIGNAL(testSignal()),SLOT(testSlot()));
    connect(this,SIGNAL(applyNewScreenGeometry()),qmlConnector,SLOT(applyScreenRect()));
    connect(qmlConnector,SIGNAL(connectServer(QString,int)),SLOT(setupNewConnection(QString,int)));
    connect(qmlConnector,SIGNAL(disconnectServer()),SLOT(disconnectServer()));
    connect(qmlConnector,SIGNAL(insertPlugin()),SLOT(pluginsInsert()));
    connect(this,SIGNAL(setScreenGeometry(QVariant,QVariant,QVariant,QVariant)),qmlConnector,SLOT(setDisplayRect(QVariant,QVariant,QVariant,QVariant)));
    connect(this,SIGNAL(displayVideoStream(QVariant)),qmlConnector,SLOT(displayStream(QVariant)));
    connect(qmlBase,SIGNAL(quit()),dataStore,SLOT(quit()));
}

void StreamerFrontend::setupNewConnection(QString ip, int port){
    setupInputSocket(ip,(quint16)(port));
}

void StreamerFrontend::disconnectServer(){
    if(!g_connectedState)
        return;
    inputPipe->handleClientDisconnect();
//    delete inputPipe;
    g_connectedState = false;
}

void StreamerFrontend::socket_disconnectServer(){
//    qmlDisconnectedStatus();
    delete inputPipe;
    g_connectedState = false;
}

void StreamerFrontend::sendEvent(int type, int val1, int val2){
    if(g_connectedState)
        inputPipe->transmitData(inputPipeHandler->createTransmission(type,val1,val2));
}

void StreamerFrontend::sendEvent(qint16 type, qint64 val1, qint64 val2){
    if(g_connectedState)
        inputPipe->transmitData(inputPipeHandler->createTransmission(type,val1,val2));
}

void StreamerFrontend::handleCommandSignal(qint8 command,qint16 value){
    switch(command){
    case StreamerEnums::COMMAND_C_SET_STREAM:{
        bool ok = false;
        QByteArray* urlData = new QByteArray();
        requestData(value,urlData,&ok);
        qDebug() << urlData->toBase64();
        if(ok)
            displayVideoStream(QUrl(QString(*urlData)));
        break;
    }
    case StreamerEnums::COMMAND_C_SET_S_O_X:{
        setScreenGeometry(value,-1,-1,-1);break;
    }
    case StreamerEnums::COMMAND_C_SET_S_O_Y:{
        setScreenGeometry(-1,value,-1,-1);break;
    }
    case StreamerEnums::COMMAND_C_SET_S_S_W:{
        setScreenGeometry(-1,-1,value,-1);break;
    }
    case StreamerEnums::COMMAND_C_SET_S_S_H:{
        setScreenGeometry(-1,-1,-1,value);break;
    }
    case StreamerEnums::COMMAND_C_SET_RECT:{
        applyNewScreenGeometry();
    }
    case StreamerEnums::COMMAND_C_SET_OVERLAY:{
        bool ok = false;
        QByteArray* compData = new QByteArray();
        requestData(value,compData,&ok);
        qDebug() << compData->toBase64();
        if(ok&&compData->size()>0){
            QQmlComponent *overlay = new QQmlComponent(qmlBase->rootContext());
//            overlay->setData(*compData,QUrl(":/overlay/"));
        }
    }
    }
}

void StreamerFrontend::pluginsInsert(){
    QFile pluginsFile("plugins.json");
    if(!pluginsFile.open(QIODevice::ReadOnly)){
        qDebug()<<"Plugin loader: " << "Failed to open file";
        return;
    }
    QJsonDocument pluginsDoc;
    QJsonParseError error;
    pluginsDoc = QJsonDocument::fromJson(pluginsFile.readAll(),&error);
    if(error.error!=QJsonParseError::NoError){
        qDebug()<<"Plugin loader: "<<error.errorString();
        return;
    }
    QList<QVariant> plugList = pluginsDoc.array().toVariantList();
    for(int i=0;i<plugList.size();i++){
        QPluginLoader libhandle(plugList.at(i).toString(),this);
        if(libhandle.load()&&libhandle.isLoaded()){
            QThread *providerThread = new QThread(this);
            InputPluginInterface *provider = qobject_cast<InputPluginInterface*>(libhandle.instance());
            provider->testFunc();
            connect(provider,SIGNAL(inputSignal(qint16,qint64,qint64)),SLOT(sendEvent(qint16,qint64,qint64)));
            connect(provider,SIGNAL(inputSignal(qint16,qint64,qint64)),SLOT(printPack(qint16,qint64,qint64)));
            provider->moveToThread(providerThread);
            connect(providerThread,SIGNAL(started()),provider,SLOT(start()));
            inputStack.append(provider);
            threadStack.append(providerThread);
            providerThread->start();
        }else{
            qDebug()<<"Plugin loader: " << "Failed to load library:" << plugList.at(i).toString();
            qDebug()<<"Plugin loader: " << "Error:" << libhandle.errorString();
        }
    }
}
