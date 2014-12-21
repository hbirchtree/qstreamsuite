#ifndef STREAMERFRONTEND_H
#define STREAMERFRONTEND_H

#include "../socketworker.h"
#include "../streamer_enums.h"
#include "../jsoncomm.h"
#include "datastore.h"
#include "eventcapture.h"
#include "analogstick.h"
#include "trackpoint.h"

#include "inputplugininterface.h"

#include <QWidget>
#include <QGridLayout>
#include <QMessageBox>

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtQml/QtQml>

#include <QTcpSocket>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QFile>
#include <QPluginLoader>

#include <QEventLoop>

namespace Ui {
class StreamerFrontend;
}

class StreamerFrontend : public QWidget
{
    Q_OBJECT

public:
    explicit StreamerFrontend(QWidget *parent = 0);
    ~StreamerFrontend();

private:

    //Input
    void setupInputSocket(QString ipAddress, quint16 port);
    QList<InputPluginInterface*> inputStack;
    QList<QThread*> threadStack;

    //Viewport
//    QGraphicsScene *scene;
//    StreamerView *view;
    void setupQMLUI(QUrl qmlUi);
    QQmlApplicationEngine *qmlBase;
    QObject *qmlConnector;
//    QQuickWidget *statusWidget;
//    QGraphicsProxyWidget *statusItem;

    //Networking
    bool g_connectedState;
    SocketWorker *inputPipe;
    JsonComm *inputPipeHandler;
    DataStore *dataStore;

signals:
    void updateStatusText(QVariant message); //QVariant in order to work with the QML slot
    void qmlConnectedStatus();
    void qmlDisconnectedStatus();
    void displayVideoStream(QVariant url);
    void setScreenGeometry(QVariant x,QVariant y,QVariant w,QVariant h);
    void applyNewScreenGeometry();
    void requestData(qint8 slot,QByteArray *data,bool *complete);

public slots:


private slots:
    void displayError(QString errorString){qDebug() << errorString;updateStatusText(errorString);}
    void forwardStatusText(QString message){emit updateStatusText(message);}

    void printPack(int t,int v1,int v2){qDebug() << t << v1 << v2;}
    void printPack(qint16 t,qint64 v1,qint64 v2){qDebug() << t << v1 << v2;}
    void printPack(qint16 d,QByteArray *data){qDebug() << d << QString::fromLocal8Bit(*data);}
    void printPack(qint16 c,qint16 v){qDebug() << c << v;}

    void testSlot(){updateStatusText("TESTICLES");}
    void pluginsInsert();

    void sendEvent(int type,int val1,int val2);
    void sendEvent(qint16 type,qint64 val1,qint64 val2);

    void setupNewConnection(QString ip, int port);
    void disconnectServer();
    void socket_disconnectServer();

    void handleCommandSignal(qint8 command, qint16 value);
};

#endif // STREAMERFRONTEND_H
