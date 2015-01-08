#include "inputhandler.h"
#include <QDebug>

InputHandler::InputHandler(QList<QVariant> inconfiguration, QObject *parent) :
    QObject(parent)
{
    configuration = new QList<QVariant>(inconfiguration);
//    qDebug() << *configuration;

    setupInputAssociations();
}

InputHandler::~InputHandler(){
    delete configuration;
    foreach(InputHandlerObjectInterface* handler,inputAssoc.values())
        delete handler;
}

void InputHandler::setupInputAssociations(){
    for(int i=0;i<configuration->size();i++){
        vhash instance = configuration->value(i).toHash();
        vlist* mapping = new QList<QVariant>(instance.value("mapping").toList());
        if(instance.value("enabled").toBool()&&(!mapping->isEmpty())){
            QPluginLoader libhandle(instance.value("library").toString(),this);
            InputHandlerObjectInterface *handler;
            if(libhandle.load()&&libhandle.isLoaded()){
                handler = qobject_cast<InputHandlerObjectInterface*>(libhandle.instance());
                handler->testFunc();
                connect(handler,SIGNAL(pass__input(qint16,qint64,qint64)),SLOT(handleInput(qint16,qint64,qint64)));
                insertHandler(handler,mapping);
            }else{
                qDebug() << "Failed to load library:" << libhandle.errorString();
            }
        }
    }
}

void InputHandler::insertHandler(InputHandlerObjectInterface *handler, QList<QVariant> *association){
    for(int i=0;i<association->size();i++){
        QVariant key = association->at(i);
        if(key.type()==QVariant::String){
            QString s_key = key.toString();
            if(s_key=="default-kbd"){
                inputAssoc[QEvent::KeyPress] = handler;
                inputAssoc[QEvent::KeyRelease] = handler;
            }else if(s_key=="default-mouse"){
                inputAssoc[QEvent::MouseMove] = handler;
                inputAssoc[QEvent::MouseButtonPress] = handler;
                inputAssoc[QEvent::MouseButtonRelease] = handler;
                inputAssoc[QEvent::TabletMove] = handler;
                inputAssoc[QEvent::Wheel] = handler;
            }
        }else if(key.type()==QVariant::Double){
            int i_key = key.toInt();
            inputAssoc[i_key] = handler;
        }else{
            qDebug() << "dropped association: " << key;
        }
    }
}

void InputHandler::handleInput(qint16 t, qint64 v1, qint64 v2){
    if(inputAssoc.contains(t)){
        inputAssoc.value(t)->take__input(t,v1,v2);
    }else{
        qDebug() << "dropped event: " << t << v1 << v2;
    }
}

void InputHandler::handlePluginRequest(QStringList options){
    InputHandlerObjectInterface* sourcePlugin = qobject_cast<InputHandlerObjectInterface*>(sender());

}
