#ifndef EVENTCAPTURE_H
#define EVENTCAPTURE_H

#include <QQuickItem>
#include "math.h"

class EventCapture : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool tapClick READ tapClick WRITE setTapClick NOTIFY tapClickChanged)
    Q_PROPERTY(bool mouseTracking READ mouseTracking WRITE setMouseTracking NOTIFY mouseTrackingChanged)
    Q_PROPERTY(bool keyCapture READ keyCapture WRITE setKeyCapture NOTIFY keyCaptureChanged)
    Q_PROPERTY(float xscale READ xscale WRITE setXscale NOTIFY xscaleChanged) //Used to fit screen area to target, will send appropriate events this way instead of going 1:1
    Q_PROPERTY(float yscale READ yscale WRITE setYscale NOTIFY yscaleChanged)
    Q_PROPERTY(float relDec READ relDec WRITE setRelDec NOTIFY relDecChanged) //Deceleration for relative mouse movement, as it otherwise will have an acceleration
public:
    EventCapture()
    {
        b_tapClick = true;
        b_keyCapture = true;
        setAcceptedMouseButtons(Qt::AllButtons);
        setAcceptHoverEvents(true);
        r_xscale = 1;
        r_yscale = 1;
        f_relDec = 4;
    }
    ~EventCapture(){

    }

    bool b_tapClick;
    bool tapClick() const{
        return b_tapClick;
    }
    void setTapClick(const bool &tapClick_){
        b_tapClick = tapClick_;
    }

    bool mouseTracking() const{
        return acceptHoverEvents();
    }
    void setMouseTracking(const bool &mouseTracking_){
        setAcceptHoverEvents(mouseTracking_);
    }

    bool b_keyCapture;
    bool keyCapture() const{
        return b_keyCapture;
    }
    void setKeyCapture(const bool &keyCapture_){
        b_keyCapture = keyCapture_;
    }

    float r_xscale;
    float r_yscale;
    float xscale() const{
        return r_xscale;
    }
    float yscale() const{
        return r_yscale;
    }
    void setXscale(const float &xscale_){
        r_xscale = xscale_;
    }
    void setYscale(const float &yscale_){
        r_yscale = yscale_;
    }

    float f_relDec;
    float relDec() const{
        return f_relDec;
    }
    void setRelDec(const float &dec_){
        f_relDec = dec_;
    }

private:
    //Keys/buttons send a signal with the following data: type,keycode,modifier(s)
    //Movement/QPoint data send a signal with the following data: type,x,y
    QPoint last_point;

    void keyPressEvent(QKeyEvent *event){
        if((!event->isAutoRepeat())&&b_keyCapture)
            emit keyPressed(event->type(),event->key(),event->modifiers());
        event->accept();
    }
    void keyReleaseEvent(QKeyEvent *event){
        if((!event->isAutoRepeat())&&b_keyCapture)
            emit keyPressed(event->type(),event->key(),event->modifiers());
        event->accept();
    }

    void hoverMoveEvent(QHoverEvent *event){
        if(!b_tapClick)
            emit mouseMovedAbs(QMouseEvent::MouseMove,event->pos().x()*r_xscale,event->pos().y()*r_yscale);
    }
    void mouseMoveEvent(QMouseEvent *event){
        if(!b_tapClick){
            emit mouseMovedAbs(event->type(),event->pos().x()*r_xscale,event->pos().y()*r_yscale);
        }else{
            emit mouseMovedRel(QEvent::TabletMove,(event->pos().x()-last_point.x())*r_xscale,(event->pos().y()-last_point.y())*r_yscale);
            last_point = event->pos();
        }
    }

    /* How to implement different ways for it to move:
     * Analog movement-ish:
     * movement:
     * emit mouseMovedRel(QEvent::TabletMove,(event->pos().x()-last_point.x())/f_relDec,(event->pos().y()-last_point.y())/f_relDec);
     *
     * press:
     * last_point = event->pos();
     *
     * Touchpad movement-ish:
     * movement:
     * emit mouseMovedRel(QEvent::TabletMove,(event->pos().x()-last_point.x())*r_xscale,(event->pos().y()-last_point.y())*r_yscale);
     * last_point = event->pos();
     *
     * press:
     * last_point = event->pos();
    */

    void mousePressEvent(QMouseEvent *event){
        if(!b_tapClick){
            emit keyPressed(event->type(),event->button(),0);
        }else
            last_point = event->pos();
    }
    void mouseReleaseEvent(QMouseEvent *event){
        if(!b_tapClick)
            emit keyPressed(event->type(),event->button(),0);
    }
    void mouseDoubleClickEvent(QMouseEvent *event){
        if(b_tapClick){
            emit keyPressed(QMouseEvent::MouseButtonPress,event->button(),0);
            emit keyPressed(QMouseEvent::MouseButtonRelease,event->button(),0);
        }
    }

    void wheelEvent(QWheelEvent *event){
        emit wheelMoved(event->type(),event->angleDelta());
    }

signals:
    void keyPressed(int type,int keycode,int modifiers);
    void mouseMovedAbs(int type, int x, int y);
    void mouseMovedRel(int type, int x, int y);
    void wheelMoved(int type,QPoint delta);

    void tapClickChanged();
    void mouseTrackingChanged();
    void keyCaptureChanged();

    void xscaleChanged();
    void yscaleChanged();

    void relDecChanged();
};

#endif // EVENTCAPTURE_H
