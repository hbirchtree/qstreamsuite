#include "streamerview.h"
#include <QDebug>

StreamerView::StreamerView(QWidget *parent) :
    QGraphicsView(parent)
{
    setMouseTracking(true);
}

void StreamerView::keyPressEvent(QKeyEvent *keyEvent){
    emit keyCaptured(keyEvent->type(),keyEvent->key());
}

void StreamerView::keyReleaseEvent(QKeyEvent *keyEvent){
    emit keyCaptured(keyEvent->type(),keyEvent->key());
}

void StreamerView::mouseMoveEvent(QMouseEvent *mEvent){
    emit pointCaptured(mEvent->type(),mEvent->pos());
}

void StreamerView::mousePressEvent(QMouseEvent *mEvent){
    emit mouseBtnCaptured(mEvent->type(),mEvent->button());
}

void StreamerView::mouseReleaseEvent(QMouseEvent *mEvent){
    emit mouseBtnCaptured(mEvent->type(),mEvent->button());
}

void StreamerView::wheelEvent(QWheelEvent *wEvent){
    emit pointCaptured(wEvent->type(),wEvent->angleDelta());
}
