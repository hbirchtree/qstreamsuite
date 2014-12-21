#ifndef DATASTORE_H
#define DATASTORE_H

#include <QThread>
#include <QHash>
#include <QVariant>

class DataStore : public QThread
{
    Q_OBJECT
public:
    explicit DataStore(QObject *parent = 0);
    ~DataStore();

public slots:
    void set_data(qint8 slot, QByteArray *data);
    void get_data(qint8 slot,QByteArray *data, bool *complete);

private:
    QHash<qint8,QByteArray> *dataStorage;
};

#endif // DATASTORE_H
