#ifndef CAPTURINGINTERFACE
#define CAPTURINGINTERFACE

#include <QObject>
#include <QStringList>
#include <libavcodec/avcodec.h>

class CaptureInterface : public QObject
{
    Q_OBJECT
public:
    ~CaptureInterface() = 0;
    virtual void startCapture() = 0;
    virtual void stopCapture() = 0;
    virtual AVCodec getSpec() = 0;
    virtual QString pluginName(){return "Generic media plugin";}
signals:
    void newBuffer(char data[]);
    void newBuffer(qint64 timestamp,char data[]);
    void requestUserInput(QStringList options,QString description);
public slots:
    virtual void receiveUserInput(QString option){}
};
#define CaptureInterfaceIID "CaptureInterface/1.2"
Q_DECLARE_INTERFACE(CaptureInterface,CaptureInterfaceIID)

#endif // CAPTURINGINTERFACE

