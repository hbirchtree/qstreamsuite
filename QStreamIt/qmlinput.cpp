#include "qmlinput.h"
#include <QDebug>

QMLInput::QMLInput(QUrl import, QObject *parent) :
    QObject(parent)
{
    ready = false;
    engine = new QQmlApplicationEngine(this);
    engine->load(import);
}

QList<QVariant>* QMLInput::fetchValues(QList<QVariant> *values){
    valuesTable = new QList<QVariant>();
    ready = true;
    QObject *rootObj = engine->rootObjects().at(0);
    QEventLoop waiter;
    connect(this,SIGNAL(finishedQuery()),&waiter,SLOT(quit()));
    connect(engine,SIGNAL(quit()),&waiter,SLOT(quit()));
    connect(engine,SIGNAL(destroyed()),&waiter,SLOT(quit()));
    connect(rootObj,SIGNAL(receiveString(int,QString)),this,SLOT(receiveString(int,QString)));
    connect(rootObj,SIGNAL(receiveInt(int,int)),this,SLOT(receiveInt(int,int)));
    connect(rootObj,SIGNAL(finishedQuery()),&waiter,SLOT(quit()));
    waiter.exec();
    if(valuesTable->size()>values->size())
        for(int i=values->size();i<valuesTable->size();i++)
            valuesTable->removeAt(i);
    engine->quit();
    delete engine;
    return valuesTable;
}

void QMLInput::receiveInt(int enumerator, int qmlInt){
    if(ready){
        valuesTable->insert(enumerator,qmlInt);
    }
}

void QMLInput::receiveString(int enumerator, QString qmlString){
    if(ready){
        valuesTable->insert(enumerator,qmlString);
    }
}

