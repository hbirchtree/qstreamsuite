#ifndef CAPTURINGINTERFACE
#define CAPTURINGINTERFACE

#include <QMediaMetaData>
#include <QObject>
#include <QStringList>

class CaptureInterface : public QObject
{
    Q_OBJECT
public:
    ~CaptureInterface() = 0;
    virtual void startCapture() = 0;
    virtual void stopCapture() = 0;
    virtual QMediaMetaData getMediaSpec() = 0;
signals:
    void newBuffer(QByteArray data);
    void requestUserInput(QStringList options,QString description);
public slots:
    virtual void receiveUserInput(){}
};
#define CaptureInterfaceIID "CaptureInterface/1.0"
Q_DECLARE_INTERFACE(CaptureInterface,CaptureInterfaceIID)

#endif // CAPTURINGINTERFACE

