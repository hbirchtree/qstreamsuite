#ifndef QMLINPUT_H
#define QMLINPUT_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QEventLoop>

class QMLInput : public QObject
{
    Q_OBJECT
public:
    explicit QMLInput(QUrl import,QObject *parent = 0);
    QList<QVariant> *fetchValues(QList<QVariant> *values);

signals:
    void errorOccured(QString);
    void finishedQuery();

public slots:
    void receiveString(int enumerator, QString qmlString);
    void receiveInt(int enumerator, int qmlInt);

private:
    QQmlApplicationEngine *engine;
    QList<QVariant> *valuesTable;
    bool ready;
};

#endif // QMLINPUT_H
