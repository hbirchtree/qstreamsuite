#include "pulsecapture.h"
#define BUFSIZE 4096

PulseCapture::PulseCapture()
{
    spec = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
}

PulseCapture::~PulseCapture(){

}

void PulseCapture::receiveUserInput(){

}

AVCodec PulseCapture::getSpec(){
    AVCodec pulseSpec;
    pulseSpec.name = AV_CODEC_ID_PCM_S16LE;
    pulseSpec.type = AVMEDIA_TYPE_AUDIO;
}

QString PulseCapture::pluginName(){
    return internalPluginName;
}

void PulseCapture::startCapture(){
    stop_loop = false;
    /*
     * Future additions: Select which sink we want to record
     */
    int error;
    if(!(src = pa_simple_new(NULL,"QStreamer",PA_STREAM_RECORD,NULL,"Audio recording",&spec,NULL,NULL,&error))){
        qFatal(strerror(error));
        return;
    }
    char audioBuffer[BUFSIZE];
    qint64 timestamp;
    QDateTime stamper;
    for(;;){
        timestamp = stamper.currentDateTime().toMSecsSinceEpoch();
        if(pa_simple_read(src,&audioBuffer,BUFSIZE,&error)<0){
            qFatal(strerror(error));
            break;
        }
        emit newBuffer(timestamp,audioBuffer);
        if(stop_loop) break;
    }
}

void PulseCapture::stopCapture(){
    stop_loop = true;
    pa_simple_free(src);
}
