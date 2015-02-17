#ifndef LIBAVCAPTURE_H
#define LIBAVCAPTURE_H

#include "../../../QStreamServer/capturinginterface.h"
#include "libav-capture_global.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

#include <QCoreApplication>

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

    typedef struct OutputStream {
        AVStream *st;

        /* pts of the next frame that will be generated */
        int64_t next_pts;
        int samples_count;

        AVFrame *frame;
        AVFrame *tmp_frame;

        float t, tincr, tincr2;

        struct SwsContext *sws_ctx;
        struct SwrContext *swr_ctx;
    } OutputStream;

    //Constants
    const QString internalPluginName = "FFmpeg Capture";

    const qint32 audio_bitrate = 64000;
    const qint32 audio_samplerate = 44100;
    const qint8 audio_channelcount = 2;
    const AVSampleFormat audio_samplefmt = AV_SAMPLE_FMT_S16;

    const qint32 video_bitrate = 512000;
    const qint32 video_w = 1600;
    const qint32 video_h = 900;
    const qint32 video_gopsize = 12;
    const qint32 streamFramerate = 30;
    const AVPixelFormat streamPixFmt = PIX_FMT_YUV420P9;
    const qint16 swscaleFlags = SWS_BICUBIC;
    const double streamDuration = 10.0;

    //AV functions
    int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt);
    void add_stream(OutputStream *ost, AVFormatContext *oc,
                           AVCodec **codec,
                           enum AVCodecID codec_id);
    void close_stream(AVFormatContext *oc, OutputStream *ost):
    //Audio
    void open_audio(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);
    AVFrame* alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                      uint64_t channel_layout,
                                      int sample_rate, int nb_samples);
    AVFrame* get_audio_frame(OutputStream *ost);


    //Video
    void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);
    AVFrame* get_video_frame(OutputStream *ost);
    void fill_yuv_image(AVFrame *pict, int frame_index,
                               int width, int height);
    int write_video_frame(AVFormatContext *oc, OutputStream *ost);
};

#endif // LIBAVCAPTURE_H
