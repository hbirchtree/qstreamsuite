#ifndef INPUTPLUGININTERFACE
#define INPUTPLUGININTERFACE

#include <QtPlugin>

class InputPluginInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~InputPluginInterface() = 0;
    void testFunc(){qWarning("TEST");}

    virtual void js_deadzone(qint32 deadzone){}
    virtual void js_cutoff(qint32 cutoff){}

public slots:
    virtual void start() = 0;

signals:
    void inputSignal(qint16 type,qint64 val1,qint64 val2);
};

#define InputPluginInterfaceIID "com.birchtree.InputPluginInterface/1.0"

Q_DECLARE_INTERFACE(InputPluginInterface,InputPluginInterfaceIID)

#endif // INPUTPLUGININTERFACE

