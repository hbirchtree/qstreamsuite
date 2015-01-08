#include "windowsinput.h"
#include <QDebug>

WindowsInput::WindowsInput()
{
    keyTable.insert(Qt::LeftButton,VK_LBUTTON);
    keyTable.insert(Qt::RightButton,VK_RBUTTON);
    keyTable.insert(Qt::MiddleButton,VK_MBUTTON);
    keyTable.insert(Qt::Key_Backspace,VK_BACK);
    keyTable.insert(Qt::Key_Tab,VK_TAB);
    keyTable.insert(Qt::Key_Clear,VK_CLEAR);
    keyTable.insert(Qt::Key_Return,VK_RETURN);
    keyTable.insert(Qt::Key_Shift,VK_SHIFT);
    keyTable.insert(Qt::Key_Control,VK_CONTROL);
    keyTable.insert(Qt::Key_Alt,VK_MENU);
    keyTable.insert(Qt::Key_Pause,VK_PAUSE);
    keyTable.insert(Qt::Key_CapsLock,VK_CAPITAL);
    keyTable.insert(Qt::Key_Escape,VK_ESCAPE);
    keyTable.insert(Qt::Key_Space,VK_SPACE);
    keyTable.insert(Qt::Key_PageUp,VK_PRIOR);
    keyTable.insert(Qt::Key_PageDown,VK_NEXT);
}

void WindowsInput::take__input(qint16 t, qint64 v1, qint64 v2){
    INPUT inp;
    RECT desktop;
    const HWND wDesktop = GetDesktopWindow();
    GetWindowRect(wDesktop,&desktop);
    if(t==QEvent::KeyPress||t==QEvent::KeyRelease){
        inp.type = INPUT_KEYBOARD;
        inp.ki.wScan = 0;
        inp.ki.time = 0;
        inp.ki.dwExtraInfo = 0;
    }else if(QEvent::MouseButtonPress||QEvent::MouseButtonRelease){
        inp.type = INPUT_MOUSE;
        inp.mi.mouseData = 0;
        inp.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
        inp.mi.dwExtraInfo = 0;
        inp.mi.time = 0;
    }
    qDebug() << t << v1 << v2;
    switch(t){
    case QEvent::KeyPress:{
        inp.ki.wVk = translate_qt_win_key(v1);
        break;
    }
    case QEvent::KeyRelease:{
        inp.ki.wVk = translate_qt_win_key(v1);
        inp.ki.dwExtraInfo = inp.ki.dwExtraInfo | KEYEVENTF_KEYUP;
        break;
    }
    case QEvent::MouseButtonPress:{
        switch(v1){
        case Qt::LeftButton:{
            inp.mi.dwFlags = inp.mi.dwFlags | MOUSEEVENTF_LEFTDOWN;
            break;
        }
        case Qt::RightButton:{
            inp.mi.dwFlags = inp.mi.dwFlags | MOUSEEVENTF_RIGHTDOWN;
            break;
        }
        case Qt::MiddleButton:{
            inp.mi.dwFlags = inp.mi.dwFlags | MOUSEEVENTF_MIDDLEDOWN;
            break;
        }
        default:
            qDebug() << "Windows Input: Qt mbtn dropped:" << v1;
        }

        break;
    }
    case QEvent::MouseButtonRelease:{
        switch(v1){
        case Qt::LeftButton:{
            inp.mi.dwFlags = inp.mi.dwFlags | MOUSEEVENTF_LEFTUP;
            break;
        }
        case Qt::RightButton:{
            inp.mi.dwFlags = inp.mi.dwFlags | MOUSEEVENTF_RIGHTUP;
            break;
        }
        case Qt::MiddleButton:{
            inp.mi.dwFlags = inp.mi.dwFlags | MOUSEEVENTF_MIDDLEUP;
            break;
        }
        default:
            qDebug() << "Windows Input: Qt mbtn dropped:" << v1;
        }
        break;
    }
    case QEvent::MouseMove:{
        inp.type = INPUT_MOUSE;
        inp.mi.mouseData = 0;
        inp.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;
        inp.mi.dwExtraInfo = 0;
        inp.mi.dx = (v1*(0xFFFF / desktop.right));
        inp.mi.dy = (v2*(0xFFFF / desktop.bottom));
        inp.mi.time = 0;
        SendInput(1,&inp,sizeof(INPUT));
        break;
    }
    case QEvent::TabletMove:{
        inp.type = INPUT_MOUSE;
        inp.mi.mouseData = 0;
        inp.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;
        inp.mi.dwExtraInfo = 0;
        POINT p;
        if(!GetCursorPos(&p)){
            qDebug() << "Windows Input: Failed to poll mouse position";
            return;
        }
        v1+=p.x;
        v2+=p.y;
        inp.mi.dx = (v1*(0xFFFF / desktop.right));
        inp.mi.dy = (v2*(0xFFFF / desktop.bottom));
        inp.mi.time = 0;
        SendInput(1,&inp,sizeof(INPUT));
        break;
    }
    default:
        return;
    }
    SendInput(1,&inp,sizeof(INPUT));
}

DWORD WindowsInput::translate_qt_win_key(quint64 qtkey){
    if(qtkey>=Qt::Key_0 && qtkey<=Qt::Key_9){
        return qtkey - Qt::Key_0 + 0x30;
    }else if(qtkey>=Qt::Key_A && qtkey<=Qt::Key_Z){
        return qtkey - Qt::Key_A + 0x41;
    }else if(keyTable.contains(qtkey)){
        return keyTable.value(qtkey);
    }else
        qDebug() << "Windows Input: dropped Qt key:" << qtkey;
    return 0;
}
