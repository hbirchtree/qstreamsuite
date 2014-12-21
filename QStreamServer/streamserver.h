#ifndef STREAMSERVER_H
#define STREAMSERVER_H
#include "networker.h"
#include "configreader.h"
#include "../streamer_enums.h"
#include "../jsoncomm.h"
#include "inputhandler.h"

#include <QDesktopWidget>
#include <QWidget>

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHash>

#include <QEventLoop>

namespace Ui {
class StreamServer;
}

class StreamServer : public QWidget
{
    Q_OBJECT

public:
    explicit StreamServer(QString configFile, QWidget *parent = 0);
    ~StreamServer();

public slots:
    //Display
    void updateStatusText(QString message);
    void userChooseFromList(QString message, QStringList options, QString *targetString);
    void insertLogEntry(QString message);

private slots:
    void handleNewClient(SocketWorker *newSocket);
    void printByteArray(QByteArray *data);

private:
    Ui::StreamServer *ui;

    NetWorker *networker;
    SocketWorker *inputWorker;
    InputHandler *inputHandler;

    QHash<QString,QVariant> *configuration;

    //Variables
    quint16 *avPort;
    quint16 *iPort;
    QString *interface;
    QString *streamUrl;
    QByteArray *overlayData;
    bool overlayLoaded;

    //Config
    bool loadConfiguration(QString configFile);

    //Networking management
    bool initializeNetworker();
};

#endif // STREAMSERVER_H
