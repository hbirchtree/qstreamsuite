#include "capturehandler.h"
#include <QDebug>

CaptureHandler::CaptureHandler(QList<QVariant> inconfig, QObject *parent) : QThread(parent)
{
    for(QList<QVariant>::const_iterator i=inconfig.constBegin();i<inconfig.constEnd();i++){
        vhash el;
        if(!el.value("enabled").toBool())
            continue;
        QPluginLoader libload(el.value("library").toString());
        if(libload.load()&&libload.isLoaded()){
            QThread* capThread = new QThread();
            CaptureInterface* capture = qobject_cast<CaptureInterface*>(libload.instance());
            threads.append(capThread);
            plugins.append(capture);
            capture->moveToThread(capThread);
//            int spec = capture->getMediaSpec();
//            if((spec & CAPTUREINT_MEDIA_AUDIO)==CAPTUREINT_MEDIA_AUDIO){
//                connect(capture,SIGNAL(newBuffer(QByteArray)),SLOT(receiveAudioBuffer(QByteArray)));
//            }else if((spec & CAPTUREINT_MEDIA_VIDEO)==CAPTUREINT_MEDIA_VIDEO)
//                connect(capture,SIGNAL(newBuffer(QByteArray)),SLOT(receiveVideoBuffer(QByteArray)));
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

void CaptureHandler::receiveAudioBuffer(QByteArray data){
    qDebug() << "audio buffer!";
}

void CaptureHandler::receiveVideoBuffer(QByteArray data){
    qDebug() << "video buffer!";
}
