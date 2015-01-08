#ifndef JSREMAP_H
#define JSREMAP_H

#include "js-remapper_global.h"
#include <map>
#include <vector>
#include <memory>
#include "../QStreamSuite/QStreamServer/inputhandlerobjectinterface.h"
#include <QFile>
#include <QVariant>
#include <QHash>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDateTime>
#include <QVector>

namespace Remap{
enum {
    //Input-related
    //Input types
    EVENT_JSAXIS,EVENT_JSBTN,
    //Button-exclusive properties
    MAP_DELAY_START,MAP_DELAY_END,
    //Axis-exclusive properties
    MAP_THRESHOLD_MIN,MAP_THRESHOLD_MAX,

    //Output types (includes input types)
    EVENT_MMOVE /*mouse move, relative*/,EVENT_KEY,
    //Properties
    MAP_EVENT_TYPE, //QEvent::Type
    MAP_EVENT_PROPERTY //The value (Axes send separate signals, there is no perspective of x and y, this is up to the user.)
};
}

class JSREMAPPERSHARED_EXPORT JsRemap : public InputHandlerObjectInterface
{

    Q_INTERFACES(InputHandlerObjectInterface)
    Q_PLUGIN_METADATA(IID InputHandlerObjectInterfaceIID)

    Q_OBJECT
    typedef std::vector<qint16> vec_btns;
    typedef std::vector<vec_btns > vec_keys;
    typedef std::map<qint8,qint64> map_properties;
//    typedef QList<qint16> vec_btns;
//    typedef QHash<qint8,qint64> map_properties;

    typedef std::map<qint16,qint64> map_pressed;

    typedef std::map<qint16,vec_keys> map_linkage;
    typedef std::map<vec_btns,map_properties> map_mapping;
//    typedef QHash<qint16,QList<QList<qint16> > > map_linkage;
//    typedef QHash<QList<qint16>,map_properties* > map_mapping;
//    typedef QHash<qint16,qint64> map_pressed;

//    typedef QPair<QList<qint16>,map_properties* > pair_mapping;

    typedef std::pair<vec_btns,map_properties> pair_mapping;
public:
    JsRemap();
    ~JsRemap();

    void take__input(qint16 t,qint64 v1,qint64 v2);
    void testFunc(){qWarning("Joystick remapper loaded!");}

signals:
    void pass__input(qint16 t,qint64 v1,qint64 v2);

public slots:
    void receiveUserSelect(QString selection){}

private:
    void createMapping(QJsonDocument *doc);
    QDateTime *ct;
    map_pressed btnsPressed; //format: key : timestamp
    map_mapping btnMappings; //format: [0,1,2] : [DELAY_START(0),DELAY_END(0),EVENT_TYPE,EVENT_PROPERTY,DATA_TYPE]
    map_mapping axMappings; //like above, with same value format, only for axes
    map_linkage btnLinks; //format: 0 : that event^
};

#endif // JSREMAP_H
