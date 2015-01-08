#ifndef WINDOWSINPUT_H
#define WINDOWSINPUT_H

#include <Windows.h>
#include <WinUser.h>
#include "windows-input_global.h"
#include "..\StreamSuite\QStreamServer\inputhandlerobjectinterface.h"
#include <QEvent>
#include <QKeyEvent>
#include <QHash>

class WINDOWSINPUTSHARED_EXPORT WindowsInput : public InputHandlerObjectInterface
{

    Q_OBJECT
    Q_INTERFACES(InputHandlerObjectInterface)
    Q_PLUGIN_METADATA(IID InputHandlerObjectInterfaceIID)
public:
    WindowsInput();
    ~WindowsInput(){}

    void take__input(qint16 t,qint64 v1,qint64 v2);
    void testFunc(){}

signals:
    void pass__input(qint16 t,qint64 v1,qint64 v2);

public slots:
    void receiveUserSelect(QString selection){}

private:
    DWORD translate_qt_win_key(quint64 qtkey);
    QHash<qint64,DWORD> keyTable;
};

#endif // WINDOWSINPUT_H
