#ifndef DIGITALKEY
#define DIGITALKEY

#include <QQuickItem>
#include "math.h"
#include <QDebug>

class DigitalKeySurface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString keySignal READ keySignal WRITE setKeySignal NOTIFY keySignalChanged)
    Q_PROPERTY(int keycode READ keycode WRITE setKeycode NOTIFY keycodeChanged)
public:
    DigitalKeySurface()
    {
        setAcceptedMouseButtons(Qt::AllButtons);
    }
    ~DigitalKeySurface(){}
    QString s_key = "";
    int i_key = 0;
    QString keySignal() const{
        return s_key;
    }
    void setKeySignal(const QString &key_){
        s_key = key_;
        QKeySequence seq(key_);
        if(seq.count()>0)
            i_key = seq[0];
    }
    int keycode() const{
        return i_key;
    }
    void setKeycode(const int &key_){
        return; //No.
    }

private:
    void mousePressEvent(QMouseEvent *event){
        newEvent(QEvent::KeyPress,i_key);
    }
    void mouseReleaseEvent(QMouseEvent *event){
        newEvent(QEvent::KeyRelease,i_key);
    }

signals:
    void newEvent(int type,int keycode);
    void keySignalChanged();
    void keycodeChanged();
};

#endif // DIGITALKEY

