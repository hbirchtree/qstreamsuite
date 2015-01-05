#ifndef ANALOGSTICK
#define ANALOGSTICK

#include <QQuickItem>
#include <QTimer>
#include "math.h"

class AnalogStickSurface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float relDec READ relDec WRITE setRelDec NOTIFY relDecChanged) //Deceleration for relative mouse movement, as it otherwise will have an acceleration
    Q_PROPERTY(QPoint stickCenter READ stickCenter WRITE setStickCenter NOTIFY stickCenterChanged)
    Q_PROPERTY(int edgePoint READ edgePoint WRITE setEdgePoint NOTIFY edgePointChanged)
    Q_PROPERTY(bool clock READ clock WRITE setClock NOTIFY clockChanged)
public:
    AnalogStickSurface()
    {
        setAcceptedMouseButtons(Qt::AllButtons);
        setAcceptHoverEvents(true);
        f_relDec = 12;
        i_edge = 100;
        analogClock.setInterval(25);
        analogClock.setTimerType(Qt::CoarseTimer);
        connect(&analogClock,SIGNAL(timeout()),SLOT(handleEvent()));
//        analogClock.start();
    }
    ~AnalogStickSurface(){

    }

    bool b_clock;
    bool clock() const{return b_clock;}
    void setClock(const bool &clock_){if(!clock_){
            analogClock.stop();
        }else{
            analogClock.start();
        }

            ;}

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
    QTimer analogClock;
//    QPoint curr_pnt;
//    QPoint past_pnt;
    int gx;
    int gy;
    void mouseMoveEvent(QMouseEvent *event){
//        curr_pnt=event->pos();
        gx = (event->pos().x()-i_edge)/f_relDec;
        gy = (event->pos().y()-i_edge)/f_relDec;
        handleEvent();
    }

    void mousePressEvent(QMouseEvent *event){} //Apparently I need this here to capture mouse movement. Meh.
    void mouseReleaseEvent(QMouseEvent *event){emit mouseReleased();/*curr_pnt=QPoint(i_edge,i_edge);*/gx=0;gy=0;}

private slots:
    void handleEvent(){
        if(gx>i_edge) gx=i_edge;
        if(gx<-i_edge) gx=-i_edge;
        if(gy>i_edge) gy=i_edge;
        if(gy<-i_edge) gy=-i_edge;
        if(gx!=0||gy!=0){
//            curr_pnt=QPoint(gx,gy);
            emit mouseMovedRel(QEvent::TabletMove,gx,gy);
        }
    }

signals:
    void mouseMovedRel(int type, int x, int y);
    void mouseReleased();

    void relDecChanged();
    void stickCenterChanged();
    void edgePointChanged();
    void clockChanged();
};

#endif // ANALOGSTICK

