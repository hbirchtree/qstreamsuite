#include "capturehandler.h"
#include <QDebug>

CaptureHandler::CaptureHandler(QList<QVariant> inconfig, QObject *parent) : QThread(parent)
{
    for(auto const it : inconfig.toStdList()){
        vhash el = it.toHash();
        if(!el.value("enabled").toBool())
            continue;
        QPluginLoader libload(el.value("library").toString());
        if(libload.load()&&libload.isLoaded()){
            QThread* capThread = new QThread();
            CaptureInterface* capture = qobject_cast<CaptureInterface*>(libload.instance());
            threads.append(capThread);
            plugins.append(capture);
            capture->moveToThread(capThread);
            qDebug() << "Connected to:" << capture->pluginName();
            connect(capture,SIGNAL(newBuffer(char[])),SLOT(newMediaBuffer(char[])));
        }else{
            qDebug() << "Failed to load library:" << libload.errorString();
        }
    }
}

void CaptureHandler::startCapture(){
    foreach(CaptureInterface* inf,plugins)
        inf->startCapture();
}

void CaptureHandler::stopCapture(){
    foreach(CaptureInterface* inf,plugins)
        inf->stopCapture();
}

CaptureHandler::~CaptureHandler()
{
    foreach(CaptureInterface* inf,plugins)
        delete inf;
    foreach(QThread* thrd,threads)
        delete thrd;
}

void CaptureHandler::newMediaBuffer(char incoming[]){

}
