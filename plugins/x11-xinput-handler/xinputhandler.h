#ifndef XINPUTHANDLER_H
#define XINPUTHANDLER_H

#include "libx11-xinput-handler_global.h"
#include <QObject>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QList>
#include <QHash>
#include <QDebug>
#include "../QStreamSuite/QStreamServer/inputhandlerobjectinterface.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

class LIBX11XINPUTHANDLERSHARED_EXPORT XinputHandler : public InputHandlerObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(InputHandlerObjectInterface)
    Q_PLUGIN_METADATA(IID InputHandlerObjectInterfaceIID)
public:
    XinputHandler(/*QObject *p = 0*/);
    ~XinputHandler();
    void take__input(qint16 t,qint64 v1,qint64 v2);

    void testFunc(){
        qWarning("It works!");
    }

signals:
    void requestUserSelect(QList<QString> *options);
    void pass__input(qint16 t,qint64 v1,qint64 v2);

public slots:
    void receiveUserSelect(QString selection);

private:
    typedef QHash<Qt::Key,quint16> keytable;
    typedef QHash<Qt::MouseButton,quint16> buttontable;
    typedef QHash<Qt::Key,bool> modtable;
    keytable QEvent_X11_map(){
        //We'd prefer to have a table where we can look up, for instance, Qt::Key_A and "instantly" get the key. We don't want to do math on it, just look up the corresponding key.
        keytable map;
        //"Special" keys
        map[Qt::Key_Escape] = XK_Escape;
        map[Qt::Key_Tab] = XK_Tab;
        map[Qt::Key_Backspace] = XK_BackSpace;
        map[Qt::Key_Return] = XK_Return;
        map[Qt::Key_Insert] = XK_Insert;
        map[Qt::Key_Pause] = XK_Pause;
        map[Qt::Key_Delete] = XK_Delete;
        map[Qt::Key_SysReq] = XK_Sys_Req;
        map[Qt::Key_Clear] = XK_Clear;
        map[Qt::Key_PageUp] = XK_Page_Up;
        map[Qt::Key_PageDown] = XK_Page_Down;
        map[Qt::Key_Menu] = XK_Menu;
        map[Qt::Key_Print] = XK_Print;
        map[Qt::Key_Home] = XK_Home;
        map[Qt::Key_End] = XK_End;

        map[Qt::Key_CapsLock] = XK_Caps_Lock;
        map[Qt::Key_ScrollLock] = XK_Scroll_Lock;
        map[Qt::Key_NumLock] = XK_Num_Lock;

        map[Qt::Key_Space] = XK_space;

        map[Qt::Key_Left] = XK_Left;
        map[Qt::Key_Right] = XK_Right;
        map[Qt::Key_Up] = XK_Up;
        map[Qt::Key_Down] = XK_Down;
        //Modifiers
        map[Qt::Key_Control] = XK_Control_L;
        map[Qt::Key_Shift] = XK_Shift_L;
        map[Qt::Key_Alt] = XK_Alt_L;
        map[Qt::Key_AltGr] = XK_ISO_Level3_Shift;
        map[Qt::Key_Multi_key] = XK_Multi_key;
        map[Qt::Key_Super_L] = XK_Super_L;
        map[Qt::Key_Super_R] = XK_Super_R;
        map[Qt::Key_Meta] = XK_Meta_L;
        //Keypad - Qt does not know what XK_KP_[1,9] is. Shift.
        map[Qt::Key_Enter] = XK_KP_Enter;
        map[Qt::Key_multiply] = XK_KP_Multiply;
        map[Qt::Key_Minus] = XK_KP_Subtract;
        map[Qt::Key_Plus] = XK_KP_Add;
        //Range for FN keys: Q:[0x01000030,0x01000052] X:[0xffbe,0xffc7]&&[0xffc8,0xffe0,2]
        //Range for upper-case alphanumeric characters: Q:[0x40,0x5a] X:[0x0041,0x5a]
        //Miscellaneous keys
        map[Qt::Key_AE] = XK_AE;
        map[Qt::Key_Aring] = XK_Aring;

        //Android-specific - mostly for testing purposes, but is also useful. We might want to allow remapping of this key by the user. (Client-side, that is.)
        map[Qt::Key_Back] = XK_Super_L;

        return map;
    }
    buttontable QEvent_X11_btnmap(){
        buttontable map;
        map[Qt::LeftButton] = 1;
        map[Qt::RightButton] = 3;
        map[Qt::MiddleButton] = 2;
        map[Qt::BackButton] = 8;
        map[Qt::ForwardButton] = 9;
        return map;
    }

    buttontable qevent_x11_btntable = QEvent_X11_btnmap();
    keytable qevent_x11_keytable = QEvent_X11_map();

    modtable x11_modifiers;

    quint16 translate_qt_x11_key(bool pressed,quint64 qtkey,quint64 modifiers);

    //Settings pertaining to X, these need to be set when the library is loaded.
    Display *display;
    Window context;

};

#endif // XINPUTHANDLER_H
