#ifndef STREAMERVIEW_H
#define STREAMERVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>

class StreamerView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit StreamerView(QWidget *parent = 0);

signals:
    void keyCaptured(QEvent::Type,int); //We do not know if the key input is valid. As such we should not try to cast it into an enum. (Yet)
    void mouseBtnCaptured(QEvent::Type,int);
    void pointCaptured(QEvent::Type,QPoint);

public slots:

private:
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);
    void mouseMoveEvent(QMouseEvent *mEvent);
    void mousePressEvent(QMouseEvent *mEvent);
    void mouseReleaseEvent(QMouseEvent *mEvent);
    void wheelEvent(QWheelEvent *wEvent);

};

#endif // STREAMERVIEW_H
