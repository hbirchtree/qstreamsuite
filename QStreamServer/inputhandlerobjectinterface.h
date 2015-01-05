#ifndef INPUTHANDLEROBJECTINTERFACE
#define INPUTHANDLEROBJECTINTERFACE

#include <QtPlugin>

class InputHandlerObjectInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~InputHandlerObjectInterface(){}

    virtual void take__input(qint16 t,qint64 v1,qint64 v2) = 0;
    virtual void testFunc() = 0;

signals:
    void pass__input(qint16 t,qint64 v1,qint64 v2);

public slots:
    virtual void receiveUserSelect(QString selection) = 0;
};

#define InputHandlerObjectInterfaceIID "com.birchtree.InputHandlerObjectInterface/1.1"

Q_DECLARE_INTERFACE(InputHandlerObjectInterface,InputHandlerObjectInterfaceIID)


#endif // INPUTHANDLEROBJECTINTERFACE

