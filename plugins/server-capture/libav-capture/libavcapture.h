#ifndef LIBAVCAPTURE_H
#define LIBAVCAPTURE_H

#include "../../../QStreamServer/capturinginterface.h"
#include "libav-capture_global.h"
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>

class LIBAVCAPTURESHARED_EXPORT Libavcapture : public CaptureInterface
{
    Q_INTERFACES(CaptureInterface)
    Q_PLUGIN_METADATA(IID CaptureInterfaceIID)

    Q_OBJECT

public:
    Libavcapture();
    ~Libavcapture(){}
    void startCapture();
    void stopCapture();
    AVCodec getSpec(){return AVCodec();}
    QString pluginName(){return internalPluginName;}
signals:
    void newBuffer(char data[]);
    void newBuffer(qint64 timestamp,char data[]);
    void requestUserInput(QStringList options,QString description);
public slots:
    void receiveUserInput(QString option){}
private:

    //Constants
    const double streamDuration = 0.5;
    const char streamFramerate = 30; //That cinematic experience
    const AVPixelFormat streamPixFmt = PIX_FMT_YUV420P9;
    const QString internalPluginName = "LibAV Capture";
    const short sws_flags = SWS_BICUBIC;

    const qint32 audio_bitrate = 64000;
    const qint32 audio_samplerate = 44100;
    const qint8 audio_channelcount = 2;
    const AVSampleFormat audio_samplefmt = AV_SAMPLE_FMT_S16;

    const qint32 video_bitrate = 512000;
    const qint32 video_w = 1600;
    const qint32 video_h = 900;
    const qint32 video_gopsize = 12;

    //AV functions
    AVStream* add_audio_stream(AVFormatContext* oc, enum CodecID codec_id);
    AVStream* add_video_stream(AVFormatContext* oc, enum CodecID codec_id);
};

#endif // LIBAVCAPTURE_H
