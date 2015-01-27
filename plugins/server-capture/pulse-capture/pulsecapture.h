#ifndef PULSECAPTURE_H
#define PULSECAPTURE_H

#include "pulse-capture_global.h"
#include "../../../QStreamServer/capturinginterface.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <QStringList>
#include <QDateTime>

class PULSECAPTURESHARED_EXPORT PulseCapture : public CaptureInterface
{

    Q_INTERFACES(CaptureInterface)
    Q_PLUGIN_METADATA(IID CaptureInterfaceIID)

    Q_OBJECT
public:
    PulseCapture();
    ~PulseCapture();
    void startCapture();
    void stopCapture();
    AVCodec getSpec();
    QString pluginName();
signals:
    void newBuffer(qint64 timestamp,char data[]);
    void requestUserInput(QStringList options,QString description);
public slots:
    void receiveUserInput();

private:
    const QString internalPluginName = "PulseAudio Capture";
    bool stop_loop = false;
    pa_simple *src;
    pa_sample_spec spec;
};

#endif // PULSECAPTURE_H
