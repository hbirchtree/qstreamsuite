#include "libavcapture.h"

Libavcapture::Libavcapture()
{
}

void Libavcapture::startCapture(){
    AVFormatContext *fc;
    AVOutputFormat *of;

    av_register_all();

    AVStream *as,*vs;
    qint64 a_pts,v_pts;

    of->flags = AVFMT_NOFILE;

    fc = avformat_alloc_context();
    fc->audio_codec_id = AV_CODEC_ID_AAC;
    fc->video_codec_id = AV_CODEC_ID_H264;

    vs = NULL;
    as = NULL;
    if(fc->audio_codec_id!=AV_CODEC_ID_NONE)
        as=add_audio_stream(fc,fc->audio_codec_id);
    if(fc->video_codec_id!=AV_CODEC_ID_NONE)
        vs=add_video_stream(fc,fc->video_codec_id);

    avformat_write_header(fc,NULL);
}

AVStream* Libavcapture::add_audio_stream(AVFormatContext* oc, enum CodecID codec_id){
    AVCodecContext *c;
    AVStream *st;

    st = avformat_new_stream(oc,NULL);
    if(!st)
        qFatal("failed to alloc stream");
    c = st->codec;

    c->codec_id = codec_id;
    c->codec_type = AVMEDIA_TYPE_AUDIO;

    c->sample_fmt = audio_samplefmt;
    c->bit_rate = audio_bitrate;
    c->sample_rate = audio_samplerate;
    c->channels = audio_channelcount;

    return st;
}

AVStream* Libavcapture::add_video_stream(AVFormatContext* oc, enum CodecID codec_id){
    AVCodecContext *c;
    AVStream *st;

    st = avformat_new_stream(oc,NULL);
    if(!st)
        qFatal("failed to alloc stream");

    c = st->codec;
    c->codec_id = codec_id;
    c->codec_type = AVMEDIA_TYPE_VIDEO;

    c->bit_rate = video_bitrate;
    c->width = video_w;
    c->height = video_h;
    c->time_base.den = streamFramerate;
    c->time_base.num = 1;
    c->gop_size = video_gopsize;
    c->pix_fmt = streamPixFmt;
    if(c->codec_id==AV_CODEC_ID_MPEG2VIDEO)
        c->max_b_frames=2;
    if(c->codec_id==AV_CODEC_ID_MPEG1VIDEO)
        c->mb_decision=2;

    return st;
}

void Libavcapture::stopCapture(){
}
