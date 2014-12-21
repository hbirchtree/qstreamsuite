#ifndef JOYSTICKINPUT_H
#define JOYSTICKINPUT_H

#include <QtPlugin>
#include "../QStreamSuite/QStreamIt/inputplugininterface.h"
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL.h>
#include "math.h"

class JoystickInput : public InputPluginInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_INTERFACES(InputPluginInterface)
    Q_PLUGIN_METADATA(IID InputPluginInterfaceIID)
#endif // QT_VERSION >= 0x050000

public:
    JoystickInput();
    ~JoystickInput(){}
    void testFunc(){qWarning("Joystick plugin inserted!");}

public slots:
    void start();

signals:
    void inputSignal(qint16 type,qint64 val1,qint64 val2);
    //JS device:
    //axes: 34[axis], value, 0
    //button: 33[8 (down),9 (up)], btn, 0

private:
    bool device_occupied;
    qint32 deadzone = 5000;

    SDL_Joystick* jsdev;
    SDL_GameController* pad;
};

#endif // JOYSTICKINPUT_H
