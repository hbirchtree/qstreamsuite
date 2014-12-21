#include "datastore.h"
#include <QDebug>

/*
 * With this class, we will make use of both commands and data transfer.
 * 256 slots allow the client to store data that may be applied to, for instance,
 *  the interface. The server may give the client a QML Component as overlay in
 *  order to better control the desktop or something else. This will be a simple
 *  deposit-and-retrieve system for now.
*/

DataStore::DataStore(QObject *parent) : QThread(parent)
{
    dataStorage = new QHash<qint8,QByteArray>();
}

DataStore::~DataStore()
{
delete dataStorage;
}

void DataStore::set_data(qint8 slot, QByteArray *data){
    qDebug() << "setting data";
//    QByteArray *newContainer = new QByteArray(*data);
    dataStorage->insert(slot,*data);
}

void DataStore::get_data(qint8 slot, QByteArray *data,bool *complete){
    *complete = true;
    *data = dataStorage->value(slot);
}
