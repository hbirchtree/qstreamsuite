#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QVariant>
#include <QHash>
#include <QList>
#include <QFile>

class ConfigReader : public QObject
{
    Q_OBJECT
public:
    explicit ConfigReader(QString configFile,QObject *parent = 0);
    QHash<QString,QVariant> *parseJsonFile();

signals:
    void errorEncountered(QString);

public slots:

private:
    QString configFilename;
    QJsonDocument* openJsonFile(QString fileName);
    QList<QVariant> parseJArray(const QJsonArray &array);
    QHash<QString, QVariant> parseJObject(const QJsonObject &object);
};

#endif // CONFIGREADER_H
