#include "streamserver.h"
#include "ui_streamserver.h"
#include <QDebug>

StreamServer::StreamServer(QString configFile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamServer)
{
    ui->setupUi(this);

    iPort = new quint16(0); //It is easier to initialize these rather than checking if they are valid.
    avPort = new quint16(0);
    interface = new QString();
    streamUrl = new QString();
    overlayLoaded=false;

    loadConfiguration(configFile);
    if(!initializeNetworker())
        insertLogEntry(tr("TCP: Failed to initialize. Program will not function correctly."));
}

StreamServer::~StreamServer()
{
    delete ui;
}

bool StreamServer::initializeNetworker(){
    try {
        networker = new NetWorker(this);
        connect(networker,SIGNAL(reportStatusText(QString)),SLOT(updateStatusText(QString)));
        connect(networker,SIGNAL(requestUserSelectFromList(QString,QStringList,QString*)),SLOT(userChooseFromList(QString,QStringList,QString*)));
        connect(networker,SIGNAL(reportLogEntry(QString)),SLOT(insertLogEntry(QString)));
        connect(networker,SIGNAL(emitSocket(SocketWorker*)),SLOT(handleNewClient(SocketWorker*)));
        networker->setupServer(*iPort,interface);
    }
    catch(...){
        return false;
    }
    return true;
}

void StreamServer::handleNewClient(SocketWorker *newSocket){
    insertLogEntry("Sent welcome message");
    inputWorker = newSocket;
//    connect(inputWorker,SIGNAL(dataReceived(QByteArray*)),SLOT(printByteArray(QByteArray*)));
    connect(inputWorker,SIGNAL(reportLogEntry(QString)),SLOT(insertLogEntry(QString)));

    inputWorker->start();

    inputHandler = new InputHandler(configuration->value("input-handlers").toList(),this);
    connect(inputWorker,SIGNAL(newInputSignal(qint16,qint64,qint64)),inputHandler,SLOT(handleInput(qint16,qint64,qint64)),Qt::QueuedConnection);
    connect(inputWorker,SIGNAL(newCommandSignal(qint8,qint64)),SLOT(handleCommandSignal(qint8,qint64)));
    connect(inputWorker,SIGNAL(dataReceived(QByteArray*)),inputWorker,SLOT(interpretTransmission(QByteArray*)));

    captureHandle = new CaptureHandler(configuration->value("output-handlers").toList(),this);
    connect(inputWorker,SIGNAL(clientConnected()),captureHandle,SLOT(startCapture()));
    connect(inputWorker,SIGNAL(clientDisconnected()),captureHandle,SLOT(stopCapture()));
    captureHandle->start();

    QString s_url = *streamUrl;
    if(s_url.isEmpty())
        s_url="rtmp://flash.oit.duke.edu/vod/_definst_/test/Wildlife2.flv";
    inputWorker->sendPacket(1,s_url); //We send the stream URL to the client's data storage
    inputWorker->sendPacket(StreamerEnums::COMMAND_C_SET_STREAM,1); //We request that the client passes this URL to the stream player. Simple, right?
    QDesktopWidget dw;
    int currentScreen = dw.primaryScreen();
    inputWorker->sendPacket(StreamerEnums::COMMAND_C_REQ_SCREENS,dw.screenCount());
    inputWorker->sendPacket(StreamerEnums::COMMAND_C_SET_S_O_X,dw.screenGeometry(currentScreen).x());
    inputWorker->sendPacket(StreamerEnums::COMMAND_C_SET_S_O_Y,dw.screenGeometry(currentScreen).y());
    inputWorker->sendPacket(StreamerEnums::COMMAND_C_SET_S_S_W,dw.screenGeometry(currentScreen).width());
    inputWorker->sendPacket(StreamerEnums::COMMAND_C_SET_S_S_H,dw.screenGeometry(currentScreen).height());
    inputWorker->sendPacket(StreamerEnums::COMMAND_C_SET_RECT,qint64(0));
    if(overlayLoaded){
        inputWorker->sendPacket(2,overlayData);
        inputWorker->sendPacket(StreamerEnums::COMMAND_C_SET_OVERLAY,2);
    }
}

void StreamServer::handleCommandSignal(qint8 command,qint64 value){
    switch(command){
    case StreamerEnums::COMMAND_C_PING_LATENCY:{
        inputWorker->sendPacket(StreamerEnums::COMMAND_C_PING_LATENCY,value);
        break;
    }
    }
}

bool StreamServer::loadConfiguration(QString configFile){
    ConfigReader *configreader = new ConfigReader(configFile,this);
    connect(configreader,SIGNAL(errorEncountered(QString)),SLOT(insertLogEntry(QString)));
    configuration = configreader->parseJsonFile();
    if(configuration->isEmpty()){
        emit insertLogEntry(tr("ConfigReader: Configuration file is empty. Will not be able to load preferences."));
        return false;
    }
    foreach(QString akey,configuration->keys())
        if(akey=="streamer-config"){
            QHash<QString,QVariant> *instance = new QHash<QString,QVariant>(configuration->value(akey).toHash());
            foreach(QString bkey,instance->keys()){
                if(bkey=="i-port")
                    iPort = new quint16(instance->value(bkey).toDouble());
                if(bkey=="av-port")
                    avPort = new quint16(instance->value(bkey).toDouble());
                if(bkey=="interface")
                    interface = new QString(instance->value(bkey).toString());
                if(bkey=="stream-location")
                    streamUrl = new QString(instance->value(bkey).toString());
                if(bkey=="touch-overlay"){
                    qDebug() << instance->value(bkey).toString();
                    QFile overlayFile(instance->value(bkey).toString());
                    if(overlayFile.open(QIODevice::ReadOnly)){
                        overlayData = new QByteArray(overlayFile.readAll());
                        if(overlayData->size()>0)
                            overlayLoaded=true;
                    }else{
                        qDebug() << "Failed to load QML component";
                    }
                }
            }
        }
    delete configreader;
    return true;
}

void StreamServer::updateStatusText(QString message){
    ui->statusLabel->setText(message);
}

void StreamServer::insertLogEntry(QString message){
    if(ui->logEdit->toPlainText().isEmpty()){
        ui->logEdit->setPlainText(message);
    }else
        ui->logEdit->setPlainText(ui->logEdit->toPlainText()+"\n"+message);
    qDebug() << message;
}

void StreamServer::userChooseFromList(QString message,QStringList options,QString *targetString){
    QDialog *userDialog = new QDialog(this);
    QVBoxLayout *vrtLay = new QVBoxLayout(userDialog);
    QLabel *msgLabel = new QLabel(userDialog);
    QPushButton *selBtn = new QPushButton(userDialog);
    QComboBox *cBox = new QComboBox(userDialog);

    msgLabel->setText(message);
    selBtn->setText(tr("Confirm"));
    for(int i=0;i<options.size();i++)
        cBox->addItem(options.at(i),i);

    vrtLay->addWidget(msgLabel);
    vrtLay->addWidget(cBox);
    vrtLay->addWidget(selBtn);

    userDialog->show();
    QEventLoop waiter;
    connect(selBtn,SIGNAL(clicked()),&waiter,SLOT(quit()));
    connect(userDialog,SIGNAL(destroyed()),&waiter,SLOT(quit()));
    connect(userDialog,SIGNAL(rejected()),&waiter,SLOT(quit()));
    connect(this,SIGNAL(destroyed()),&waiter,SLOT(quit()));
    waiter.exec();
    *targetString = cBox->currentText();
    userDialog->close();
    delete userDialog;
}
