import QtQuick 2.0
import AnalogStickComponent 1.0

AnalogStick{
    signal newEvent(int type,int v1,int v2)
    id:stick
    Rectangle{
        color:"#292929"
        radius:parent.height/2
        anchors.fill:parent
        Rectangle{
            id:dot
            radius:width/2
            width:parent.width/3*2
            height:width
            color:"#696969"
        }
    }
    edgePoint: width/2
    clock:true
    onMouseMovedRel: {
        dot.x=x+dot.width/4
        dot.y=y+dot.width/4
        newEvent(type,x/width*150,y/height*150)
    }
    onMouseReleased:{
        dot.x=width/2-dot.width/2
        dot.y=height/2-dot.height/2
    }
}
