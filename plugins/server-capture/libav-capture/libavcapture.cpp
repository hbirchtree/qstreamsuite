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

    av_dump_format(fc,0,NULL,1);

    //allocate buffers here

    avformat_write_header(fc,NULL);

    for(;;){
        if(as)
            a_pts=(double)as->pts.val * as->time_base.num/as->time_base.den;
        else
            a_pts=0.0;
        if(vs)
            v_pts=(double)vs->pts.val * vs->time_base.num/vs->time_base.den;
        else
            v_pts=0.0;

        if(!as&&!vs)
            break;

        if(!vs || (vs && as && a_pts < v_pts)){
            write_aframe(fc,as);
        }else
            write_vframe(fc,vs);

        QCoreApplication::processEvents();
        if(!continueEncode)
            break;
    }
}

//Not finished!
void Libavcapture::write_aframe(AVFormatContext *oc,AVStream *st){
    AVCodecContext *c;
    AVPacket pkt;
    av_init_packet(&pkt);

    c = st->codec;

    //magic_audio_frame_source()

    pkt.size = avcodec_encode_audio(c,a_outbuf,a_outbuf_sz,a_samples);
    if(c->coded_frame && c->coded_frame->pts != AV_NOPTS_VALUE)
        pkt.pts = av_rescale_q(c->coded_frame->pts,c->time_base,st->time_base);
    pkt.flags |= AV_PKT_FLAG_KEY;
    pkt.stream_index = st->index;
    pkt.data = a_outbuf;
    if(av_interleaved_write_frame(oc,&pkt)!=0)
        qFatal("Failed writing audio frame");
}

//Not finished!
void Libavcapture::write_vframe(AVFormatContext *oc,AVStream *st){
    qint32 outsize,ret;
    AVCodecContext *c;
    static struct SwsContext *img_convert_ctx;

    c = st->codec;

    //check if there is a new frame to encode, if not, pass the old one

    if(c->pix_fmt!=PIX_FMT_YUV420P9){
        if(img_convert_ctx==NULL){
            img_convert_ctx = sws_getContext(c->width,c->height,PIX_FMT_YUV420P9,c->width,c->height,c->pix_fmt,sws_flags,NULL,NULL,NULL);
            if(img_convert_ctx==NULL)
                qFatal("Failed to initialize conversion context");
        }

        //magically acquire a video frame into *tmp_picture, *picture will hold the scaled result

        sws_scale(img_convert_ctx,tmp_picture->data,tmp_picture->linesize,0,c->height,picture->data,picture->linesize);
    }else{
        //magically acquire a video frame into *picture, this would do it in the case of no scaling required
    }
    if(oc->oformat->flags & AVFMT_RAWPICTURE){
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.flags |= AV_PKT_FLAG_KEY;
        pkt.stream_index = st->index;
        pkt.data = (u_int8_t *)picture;
        pkt.size = sizeof(AVPicture);
        
        ret = av_interleaved_write_frame(oc,&pkt);
    }else{
        outsize = avcodec_encode_video(c,v_outbuf,v_outbuf_sz,picture);
        
        if(outsize > 0){
            AVPacket pkt;
            av_init_packet(&pkt);
            
            if(c->coded_frame->pts != AV_NOPTS_VALUE)
                pkt.pts = av_rescale_q(c->coded_frame->pts,c->time_base,st->time_base);
            if(c->coded_frame->key_frame)
                pkt.flags |= AV_PKT_FLAG_KEY;
            pkt.stream_index = st->index;
            pkt.data = v_outbuf;
            pkt.size = outsize;

            ret = av_interleaved_write_frame(oc,&pkt);
        }else
            ret = 0;
    }
    if(ret!=0)
        qFatal("Error writing video frame");
    v_framecount++;
}

//Finished
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

//Finished
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
