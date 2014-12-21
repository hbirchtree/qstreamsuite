#ifndef TRACKPOINT
#define TRACKPOINT

#include <QQuickItem>
#include "math.h"
#include <QDebug>

class TrackPointComponent : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float relDec READ relDec WRITE setRelDec NOTIFY relDecChanged) //Deceleration for relative mouse movement, as it otherwise will have an acceleration
    Q_PROPERTY(QPoint stickCenter READ stickCenter WRITE setStickCenter NOTIFY stickCenterChanged)
    Q_PROPERTY(int edgePoint READ edgePoint WRITE setEdgePoint NOTIFY edgePointChanged)
public:
    TrackPointComponent()
    {
        setAcceptedMouseButtons(Qt::AllButtons);
        setAcceptHoverEvents(true);
        f_relDec = 6;
        i_edge = 100;
    }
    ~TrackPointComponent(){

    }

    float f_relDec;
    float relDec() const{
        return f_relDec;
    }
    void setRelDec(const float &dec_){
        f_relDec = dec_;
    }

    QPoint last_point;
    QPoint stickCenter() const{return last_point;}
    void setStickCenter(const QPoint &center_){last_point=center_;}

    int i_edge;
    int edgePoint() const{return i_edge;}
    void setEdgePoint(const int &center_){i_edge=center_;}

private:
    void mouseMoveEvent(QMouseEvent *event){
        int x = (event->pos().x()-last_point.x()-i_edge)/f_relDec;
        int y = (event->pos().y()-last_point.y()-i_edge)/f_relDec;
        if(x>i_edge) x=i_edge;
        if(x<-i_edge) x=-i_edge;
        if(y>i_edge) y=i_edge;
        if(y<-i_edge) y=-i_edge;
        emit mouseMovedRel(QEvent::TabletMove,x,y);
//        emit mouseMovedRel(QEvent::TabletMove,(event->pos().x()-last_point.x())/f_relDec,(event->pos().y()-last_point.y())/f_relDec);
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

    void mousePressEvent(QMouseEvent *event){} //Apparently I need this here to capture mouse movement. Meh.
    void mouseReleaseEvent(QMouseEvent *event){emit mouseReleased();}

signals:
    void mouseMovedRel(int type, int x, int y);
    void mouseReleased();

    void relDecChanged();
    void stickCenterChanged();
    void edgePointChanged();
};

#endif // TRACKPOINT

