#include "joystickinput.h"


JoystickInput::JoystickInput() :
    InputPluginInterface()
{
    //This is not *really* a plugin we want to promote. Let's make it simple.
    device_occupied = false;
    if(SDL_Init(SDL_INIT_GAMECONTROLLER)<0)
        qFatal("Failed to initalize");
}

void JoystickInput::start(){
    SDL_Event sdlEvent;
    while(true){
        while(SDL_PollEvent(&sdlEvent))
            switch(sdlEvent.type){
            case SDL_CONTROLLERDEVICEADDED:{
                if(!device_occupied){
                    device_occupied=true;
                    pad = SDL_GameControllerOpen(sdlEvent.cdevice.which);
                    if(pad){
                        jsdev = SDL_GameControllerGetJoystick(pad);
                    }
                }
                break;
            }
            case SDL_CONTROLLERDEVICEREMOVED:{
                if(device_occupied){
                    SDL_GameControllerClose(pad);
                    device_occupied=false;
                }
                break;
            }
            case SDL_CONTROLLERBUTTONDOWN:{
                inputSignal(338,sdlEvent.cbutton.button,0);
                break;
            }
            case SDL_CONTROLLERBUTTONUP:{
                inputSignal(339,sdlEvent.cbutton.button,0);
                break;
            }
            case SDL_CONTROLLERAXISMOTION:{
                if(abs(sdlEvent.caxis.value)<deadzone)
                    break;
                inputSignal(340+sdlEvent.caxis.axis,sdlEvent.caxis.value,0);
                break;
            }
            }
    }
    qWarning("Fin");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(js-input, JoystickInput)
#endif // QT_VERSION < 0x050000
