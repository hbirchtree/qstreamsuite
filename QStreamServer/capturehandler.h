#ifndef CAPTUREHANDLER_H
#define CAPTUREHANDLER_H

#include <QThread>
#include <QByteArray>
#include "capturinginterface.h"
#include <QPluginLoader>
#include <QList>
#include <QHash>
#include <QVariant>
#include <QDateTime>

class CaptureHandler : public QThread
{
    Q_OBJECT

    typedef QHash<QString,QVariant> vhash;
    typedef QList<QVariant> vlist;
public:
    explicit CaptureHandler(QList<QVariant> inconfig,QObject *parent = 0);
    ~CaptureHandler();

signals:
    void newStreamFrame(QByteArray data);

public slots:
    void startCapture();
    void stopCapture();
    void newMediaBuffer(char incoming[]);

private:

    QList<QThread*> threads;
    QList<CaptureInterface*> plugins;
};

#endif // CAPTUREHANDLER_H
