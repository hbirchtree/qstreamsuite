#include "configreader.h"
#include <QDebug>

ConfigReader::ConfigReader(QString configFile, QObject *parent) :
    QObject(parent)
{
    configFilename = configFile;
}

QHash<QString, QVariant> *ConfigReader::parseJsonFile(){
    QJsonDocument *jsonDoc = openJsonFile(configFilename);
    QHash<QString,QVariant> *outHash = new QHash<QString,QVariant>(parseJObject(jsonDoc->object()));
    delete jsonDoc;
    return outHash;
}

QJsonDocument* ConfigReader::openJsonFile(QString fileName){
    QJsonDocument *outDoc = new QJsonDocument();
    QFile *jsonFile = new QFile(fileName);

    if(!jsonFile->exists()){
        errorEncountered(tr("ConfigReader: Configuration file does not exist"));
        return outDoc;
    }
    if(!jsonFile->open(QIODevice::ReadOnly)){
        errorEncountered(tr("ConfigReader: Configuration file cannot be opened as read-only"));
        return outDoc;
    }

    QJsonParseError *jError = new QJsonParseError();
    *outDoc = outDoc->fromJson(jsonFile->readAll(),jError);
    jsonFile->close();
    if(jError->error!=QJsonParseError::NoError)
        emit errorEncountered("ConfigReader: JSON parsing error: "+jError->errorString());
    delete jError;
    delete jsonFile;
    return outDoc;
}

QList<QVariant> ConfigReader::parseJArray(QJsonArray const &array){
    QList<QVariant> outList;

    for(int i=0;i<array.size();i++){
        QJsonValue *instance = new QJsonValue(array.at(i));
        if(instance->isArray()){
            outList.append(parseJArray(instance->toArray()));
        } else if(instance->isObject()){
            outList.append(parseJObject(instance->toObject()));
        } else {
            outList.append(instance->toVariant());
        }
    }

    return outList;
}

QHash<QString, QVariant> ConfigReader::parseJObject(QJsonObject const &object){
    QHash<QString,QVariant> outHash;

    foreach(QString key, object.keys()){
        QJsonValue *instance = new QJsonValue(object.value(key));
        if(instance->isArray()){
            outHash.insert(key,parseJArray(instance->toArray()));
        } else if(instance->isObject()){
            outHash.insert(key,parseJObject(instance->toObject()));
        } else {
            outHash.insert(key,instance->toVariant());
        }
    }

    return outHash;
}
