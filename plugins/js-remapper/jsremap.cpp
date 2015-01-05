#include "jsremap.h"


JsRemap::JsRemap()
{
    ct = new QDateTime();
    QFile config("js-remap.json");
    if(config.open(QIODevice::ReadOnly)){
        QJsonDocument document;
        QJsonParseError *error = new QJsonParseError;
        document = QJsonDocument::fromJson(config.readAll(),error);
        if(error->error==QJsonParseError::NoError){
            createMapping(&document);
        }else
            qFatal("Failed to import configuration. Plugin will not work.");
        delete error;
    }
}

JsRemap::~JsRemap(){

}

void JsRemap::createMapping(QJsonDocument *doc){
    foreach(QJsonValue mappingVal,doc->array()){
        QJsonObject mapping = mappingVal.toObject();
        vec_btns input;
        map_properties btnMap;
        btnMappings.insert(pair_mapping(buttons,btnMap));
    }
}

void JsRemap::take__input(qint16 t, qint64 v1, qint64 v2){
    if(t==338){ //button presses
        map_pressed::iterator it = btnsPressed.find(v1);
        if(it!=btnsPressed.end()){
            pass__input(t,v1,v2);
            btnsPressed.insert(v1,ct->currentDateTime().toMSecsSinceEpoch());
        }else
            qWarning("Registered key that was already pressed, is server out of sync?");
    }
    if(t==339){
        map_pressed::iterator it = btnsPressed.find(v1);
        if(it!=btnsPressed.end()){
            pass__input(t,v1,v2);
            btnsPressed.erase(v1);
        }
    }
    if(t>340&&t<360){ //We will limit ourselves to 20 axes.
        pass__input(t,v1,v2);
    }
}
