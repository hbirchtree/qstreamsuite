#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QObject>
#include <QEvent>
#include <QHash>
#include <QPluginLoader>
#include "inputhandlerobjectinterface.h"

class InputHandler : public QObject
{
    Q_OBJECT

    typedef QHash<QString,QVariant> vhash;
    typedef QList<QVariant> vlist;
//    typedef QObject*(*InputHandlerObject)(void);
public:
    explicit InputHandler(QList<QVariant> inconfiguration, QObject *parent = 0);
    ~InputHandler();

private:
    QList<QVariant> *configuration;
    QHash<int,InputHandlerObjectInterface*> inputAssoc;

    void setupInputAssociations();
    void insertHandler(InputHandlerObjectInterface* handler,QList<QVariant>* association);

signals:

public slots:
    void handleInput(qint16 t,qint64 v1,qint64 v2);

};

#endif // INPUTHANDLER_H
