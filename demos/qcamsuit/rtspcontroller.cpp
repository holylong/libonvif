#include "rtspcontroller.h"
#include <QImage>
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libswscale/swscale.h"
}


RtspController::RtspController(QObject *parent)
    : QObject{parent}
{}

bool RtspController::Close(void* ctx){
    AVFormatContext *ifmt_ctx = (AVFormatContext *)ctx;
    if (NULL != ifmt_ctx) {
        avformat_close_input(&ifmt_ctx);
        ifmt_ctx = NULL;
    }
}

void RtspController::custom_log_callback(void *ptr, int level, const char *fmt, va_list vl) {
    if (level > AV_LOG_WARNING) {  // 只打印级别高于警告的信息
        av_log_default_callback(ptr, level, fmt, vl);
    }
}

void RtspController::freeAvMemory(void *data)
{
    if (data != nullptr)
    {
        av_free(data);
    }
}

QImage RtspController::createImageFromBuffer(int width, int height, uchar* rgbBufferData, int channels)
{
    return QImage(rgbBufferData, width, height, QImage::Format_RGB888, freeAvMemory);
}

bool RtspController::openRtsp(const QString& url){
    _bRunning = true;
    unsigned int    i;
    int             ret;
    int             video_st_index = -1;
    int             audio_st_index = -1;
    AVFormatContext *ifmt_ctx = NULL;
    AVPacket        pkt;
    AVStream        *st = NULL;
    char            errbuf[64];
    AVDictionary *optionsDict = NULL;
    av_register_all();                                                          // Register all codecs and formats so that they can be used.
    avformat_network_init();                                                    // Initialization of network components
    av_dict_set(&optionsDict, "rtsp_transport", "tcp", 0);                //采用tcp传输
    av_dict_set(&optionsDict, "stimeout", "2000000", 0);                  //如果没有设置stimeout，那么把ipc网线拔掉，av_read_frame会阻塞（时间单位是微妙）
    if ((ret = avformat_open_input(&ifmt_ctx, url.toStdString().c_str(), 0, &optionsDict)) < 0) {            // Open the input file for reading.
        printf("Could not open input file '%s' (error '%s')\n", url.toStdString().c_str(), av_make_error_string(errbuf, sizeof(errbuf), ret));
        Close(ifmt_ctx);
        _bOver = true;
        return false;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {                // Get information on the input file (number of streams etc.).
        printf("Could not open find stream info (error '%s')\n", av_make_error_string(errbuf, sizeof(errbuf), ret));
        Close(ifmt_ctx);
        _bOver = true;
        return false;
    }

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {                                // dump information
        av_dump_format(ifmt_ctx, i, url.toStdString().c_str(), 0);
    }

    // 设置自定义日志回调
    av_log_set_callback(custom_log_callback);

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {                                // find video stream index
        st = ifmt_ctx->streams[i];
        switch (st->codec->codec_type) {
            case AVMEDIA_TYPE_AUDIO: audio_st_index = i; break;
            case AVMEDIA_TYPE_VIDEO: video_st_index = i; break;
            default: break;
        }
    }
    if (-1 == video_st_index) {
        printf("No H.264 video stream in the input file\n");
        Close(ifmt_ctx);
        _bOver = true;
        return false;
    }

    av_init_packet(&pkt);                                                       // initialize packet.
    pkt.data = NULL;
    pkt.size = 0;
    bool nRestart = false;
    int videoindex = -1;
    int audioindex = -1;
    AVStream *pVst;
    uint8_t* buffer_rgb = NULL;
    AVCodecContext *pVideoCodecCtx = NULL;
    AVFrame         *pFrame = av_frame_alloc();
    AVFrame         *pFrameRGB = av_frame_alloc();
    int got_picture;
    SwsContext      *img_convert_ctx = NULL;
    AVCodec *pVideoCodec = NULL;
    while (_bRunning)
    {
        do {
            ret = av_read_frame(ifmt_ctx, &pkt);                                // read frames

            if (!nRestart)
            {
                for (int i = 0; i < ifmt_ctx->nb_streams; i++)
                {
                    if ((ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) && (videoindex < 0))
                    {
                        videoindex = i;
                    }
                    if ((ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) && (audioindex < 0))
                    {
                        audioindex = i;
                    }
                }
                pVst = ifmt_ctx->streams[videoindex];
                pVideoCodecCtx = pVst->codec;
                pVideoCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
                if (pVideoCodec == NULL){
                    _bOver = true;
                    return false;
                }

                if (avcodec_open2(pVideoCodecCtx, pVideoCodec, NULL) < 0){
                    _bOver = true;
                    return false;
                }

                nRestart = true;
                printf("first frame \n");
            }

            if (pkt.stream_index == videoindex)
            {
                //fprintf(stdout, "pkt.size=%d,pkt.pts=%lld, pkt.data=0x%x.", pkt.size, pkt.pts, (unsigned int)pkt.data);
                // fprintf(stdout, "pkt.size=%d,pkt.pts=%lld\n", pkt.size, pkt.pts);
                int av_result = avcodec_decode_video2(pVideoCodecCtx, pFrame, &got_picture, &pkt);

                if (av_result < 0)
                {
                    fprintf(stderr, "decode failed: inputbuf = 0x%x , input_framesize = %d\n", pkt.data, pkt.size);
                    _bOver = true;
                    return false;
                }

                if (got_picture)
                {
                    int bytes = avpicture_get_size(AV_PIX_FMT_RGB24, pVideoCodecCtx->width, pVideoCodecCtx->height);
                    if( buffer_rgb == NULL)
                        buffer_rgb = (uint8_t *)av_malloc(bytes);
                    avpicture_fill((AVPicture *)pFrameRGB, buffer_rgb, AV_PIX_FMT_RGB24, pVideoCodecCtx->width, pVideoCodecCtx->height);

                    img_convert_ctx = sws_getContext(pVideoCodecCtx->width, pVideoCodecCtx->height, pVideoCodecCtx->pix_fmt,
                                                     // pVideoCodecCtx->width, pVideoCodecCtx->height, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
                                                     pVideoCodecCtx->width, pVideoCodecCtx->height, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
                    if (img_convert_ctx == NULL)
                    {

                        printf("can't init convert context!\n");
                        _bOver = true;
                        return false;
                    }
                    sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, pVideoCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

                    emit onFrame(QImage(reinterpret_cast<uchar*>(pFrameRGB->data[0]),
                                        pVideoCodecCtx->width,
                                        pVideoCodecCtx->height,
                                        QImage::Format_RGB888,
                                        freeAvMemory));

                    sws_freeContext(img_convert_ctx);
                    //av_free(buffer_rgb);
                }

            }
        } while (ret == AVERROR(EAGAIN));

        if (ret < 0) {
            printf("Could not read frame ---(error '%s')\n", av_make_error_string(errbuf, sizeof(errbuf), ret));
            Close(ifmt_ctx);
            _bOver = true;
            return false;
        }

        if (pkt.stream_index == video_st_index) {                               // video frame
            // printf("Video Packet size = %d\n", pkt.size);
        }
        else if (pkt.stream_index == audio_st_index) {                         // audio frame
            // printf("Audio Packet size = %d\n", pkt.size);
        }
        else {
            printf("Unknow Packet size = %d\n", pkt.size);
        }

        av_packet_unref(&pkt);
    }

    Close(ifmt_ctx);
    _bOver = true;
    return true;
}
