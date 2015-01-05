#include "xinputhandler.h"

XinputHandler::XinputHandler() : InputHandlerObjectInterface()
{
    display = XOpenDisplay(0);
    if(display==NULL){
        qFatal("Failed to open display");
    }
    context = XDefaultRootWindow(display);
}

XinputHandler::~XinputHandler(){
    XCloseDisplay(display);
}

void XinputHandler::take__input(qint16 t, qint64 v1, qint64 v2){
    /* We need to:
     * - translate the event from QEvent and Qt enums to X11 ones
     * - make sure the translation does not produce incorrect input
     *
     * Also to note:
     * In X11, wheel events consist of a button press and release
     *  in order to scroll once, with dedicated mouse buttons. (4&5)
     * Mouse movements are done by way of XWarpPointer() and as such
     *  does not require XEvent.
    */
    QEvent::Type qT = static_cast<QEvent::Type>(t);

    switch(qT){
    case QEvent::MouseButtonPress:{
        Qt::MouseButton qBtn = static_cast<Qt::MouseButton>(v1);
        quint8 u_xBtn = qevent_x11_btntable[qBtn];
        XTestFakeButtonEvent(display,u_xBtn,True,CurrentTime);
        break;
    }
    case QEvent::MouseButtonRelease:{
        Qt::MouseButton qBtn = static_cast<Qt::MouseButton>(v1);
        quint8 u_xBtn = qevent_x11_btntable[qBtn];
        XTestFakeButtonEvent(display,u_xBtn,False,CurrentTime);
        break;
    }
    case 6:{ //Is supposed to be QEvent::KeyPress, name collides with X11's KeyPress
        //Here we convert the portable int to an enum type and proceed to use the enum type as a key in the hash table. If the key is not found, we will be receiving 0, the void key in X11, which obviously does not do anything, so we will just catch it in case it becomes a problem. (As in the case of the Qt::Key static_cast being iffy.)
        KeyCode xKey = XKeysymToKeycode(display,translate_qt_x11_key(true,v1,v2));
        XTestFakeKeyEvent(display,xKey,True,CurrentTime);break;
    }
    case 7:{ //Guess what this is
        KeyCode xKey = XKeysymToKeycode(display,translate_qt_x11_key(false,v1,v2));
        XTestFakeKeyEvent(display,xKey,False,CurrentTime);break;
    }
    case QEvent::MouseMove:{
        //Simple pointer warp, yay! But wait, we also want relative mouse movement support.
        XTestFakeMotionEvent(display,0,int(v1),int(v2),CurrentTime);break;
    }
    case QEvent::TabletMove:{
        //Simple pointer warp, yay! But wait, we also want relative mouse movement support.
        XTestFakeRelativeMotionEvent(display,int(v1),int(v2),CurrentTime); break;
    }
    case QEvent::Wheel:{
        //This is really bad. We'll just assume all of this works as it should.
        if(v2>0){
            XTestFakeButtonEvent(display,4,True,CurrentTime);
            XTestFakeButtonEvent(display,4,False,CurrentTime);
        }else if(v2<0){
            XTestFakeButtonEvent(display,5,True,CurrentTime);
            XTestFakeButtonEvent(display,5,False,CurrentTime);
        }else if(v1>0){
            XTestFakeButtonEvent(display,6,True,CurrentTime);
            XTestFakeButtonEvent(display,6,False,CurrentTime);
        }else if(v1<0){
            XTestFakeButtonEvent(display,7,True,CurrentTime);
            XTestFakeButtonEvent(display,7,False,CurrentTime);
        }
        break;
    }
    default: qWarning("Dropped event"); return;
    }
    XFlush(display);
}

quint16 XinputHandler::translate_qt_x11_key(bool pressed, quint64 qtkey, quint64 modifiers){
//    Qt::KeyboardModifiers qMod = static_cast<Qt::KeyboardModifiers>(int(modifiers));
    if(qtkey>=Qt::Key_Space && qtkey<Qt::Key_Escape){ //Latin-1, could we perhaps add Latin-2?
        return qtkey - Qt::Key_Space + XK_space;
    }
    if(qtkey==Qt::Key_Alt || qtkey==Qt::Key_Control || qtkey==Qt::Key_Meta || qtkey==Qt::Key_AltGr || qtkey==Qt::Key_Shift){
        Qt::Key ekey = static_cast<Qt::Key>(qtkey);
        if(pressed){
            x11_modifiers.insert(ekey,true);
        }else if(x11_modifiers.contains(ekey)){
            x11_modifiers.remove(ekey);
        }else{
            x11_modifiers.clear();
        }
    }
    if((qtkey>=Qt::Key_F1)&&(qtkey<=Qt::Key_F35)){
        return qtkey - Qt::Key_F1 + XK_F1;
    }
    return qevent_x11_keytable.value(static_cast<Qt::Key>(qtkey),0); //Default: see if it exists in the keytable. If it doesn't, 0 is returned. I hope.
}

void XinputHandler::receiveUserSelect(QString selection){
//    qDebug() << "returned" << selection;
}
