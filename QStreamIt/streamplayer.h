#ifndef STREAMPLAYER_H
#define STREAMPLAYER_H

#include <QQuickItem>

class StreamPlayer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
public:
    StreamPlayer();
    ~StreamPlayer();

    QUrl source() const{
        return u_source;
    }

private:
    QUrl u_source;

signals:
    void sourceChanged();

public slots:
    void setSource(QUrl const &source_){
        u_source = source_;
    }
};

#endif // STREAMPLAYER_H
