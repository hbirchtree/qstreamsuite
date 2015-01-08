#include "pulsecapture.h"
#define BUFSIZE 1024

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

QMediaMetaData PulseCapture::getMediaSpec(){
    spec = {
        .rate = 44100,
        .channels = 2
    };
    return (unsigned int)(CAPTUREINT_MEDIA_AUDIO|CAPTUREINT_AUDIO_44100|CAPTUREINT_AUDIO_2CHANNELS|CAPTUREINT_AUDIO_PCMS16LE);
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
    QByteArray audioBuffer;
    qint64 timestamp;
    QDateTime stamper;
    for(;;){
        timestamp = stamper.currentDateTime().toMSecsSinceEpoch();
        if(pa_simple_read(src,&audioBuffer,BUFSIZE,&error)<0){
            qFatal(strerror(error));
            break;
        }
        emit newBuffer(timestamp,audioBuffer);
        audioBuffer.clear();
        if(stop_loop) break;
    }
}

void PulseCapture::stopCapture(){
    stop_loop = true;
    pa_simple_free(src);
}
