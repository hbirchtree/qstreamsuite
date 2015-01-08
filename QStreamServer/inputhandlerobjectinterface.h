#ifndef INPUTHANDLEROBJECTINTERFACE
#define INPUTHANDLEROBJECTINTERFACE

#include <QtPlugin>
#include <QStringList>

class InputHandlerObjectInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~InputHandlerObjectInterface(){}

    virtual void take__input(qint16 t,qint64 v1,qint64 v2) = 0;
    virtual void testFunc() = 0;
    virtual QString getIdentifier(){return "com.generic.client-input";} //Aesthetic perhaps? Will not be used programmatically due to the chance of collisions

signals:
    void pass__input(qint16 t,qint64 v1,qint64 v2);
    void get__userSelect(QStringList options);

public slots:
    virtual void receiveUserSelect(QString selection) = 0;
};

#define InputHandlerObjectInterfaceIID "com.birchtree.InputHandlerObjectInterface/1.2"

Q_DECLARE_INTERFACE(InputHandlerObjectInterface,InputHandlerObjectInterfaceIID)


#endif // INPUTHANDLEROBJECTINTERFACE

