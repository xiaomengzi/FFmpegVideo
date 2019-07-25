#include "FFmpegLogic.h"

static double r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}

FFmpegLogic *FFmpegLogic::m_pInstance = nullptr;


FFmpegLogic::FFmpegLogic()
{
}

FFmpegLogic::~FFmpegLogic()
{
}

void FFmpegLogic::Uninitialize()
{
    if (nullptr != m_pInstance)
    {
        delete m_pInstance;
    }
}

FFmpegLogic * FFmpegLogic::GetInstance()
{
    if (nullptr == m_pInstance)
    {
        m_pInstance = new FFmpegLogic;
        m_pInstance->InitBasicData();
    }
    return m_pInstance;
}

void FFmpegLogic::InitBasicData()
{
    // 注册所有的格式。包括解封装格式和加封装格式。
    av_register_all();
    // 用于初始化网络。FFmpeg本身也支持解封装RTSP的数据, 如果要解封装网络数据格式，则可调用该函数。
    avformat_network_init();
}

int FFmpegLogic::OpenFile(const char *path)
{
    CloseFile();
    QMutexLocker m_lock(&mutexFFmpeg);
    // 初始化 AVFormatContext
    ic = avformat_alloc_context();

    // 打开一个文件并解析。可解析的内容包括：视频流、音频流、视频流参数、音频流参数、视频帧索引。
    int re = avformat_open_input(&ic, path, 0, 0);
    if (re != 0)
    {
        // 获取错误信息
        av_strerror(re, errorbuf, sizeof(errorbuf));
        printf("open %s failed: %s\n", path, errorbuf);
        return 0;
    }
    // 获取文件时长，单位(ms)
    totalMs = ((ic->duration / AV_TIME_BASE) * 1000);

    // 分析视频流 [remark ???]
    for (int i = 0; i < ic->nb_streams; i++)
    {
        AVCodecContext *enc = ic->streams[i]->codec;

        if (enc->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            fps = r2d(ic->streams[i]->avg_frame_rate);
            // 获取解码器 
            AVCodec *codec = avcodec_find_decoder(enc->codec_id);
            if (!codec)
            {
                printf("video code not find!\n");
                return 0;
            }
            int err = avcodec_open2(enc, codec, NULL);
            if (err != 0)
            {
                char buf[1024] = { 0 };
                av_strerror(err, buf, sizeof(buf));
                printf(buf);
                return 0;
            }
            printf("open codec success!\n");
        }
        else if (enc->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audioStream = i;
            AVCodec *codec = avcodec_find_decoder(enc->codec_id);
            if (avcodec_open2(enc, codec, NULL) < 0)
            {
                return 0;
            }
            this->sampleRate = enc->sample_rate;
            this->channel = enc->channels;
            switch (enc->sample_fmt)
            {
            case AV_SAMPLE_FMT_S16:         ///< signed 16 bits
                this->sampleSize = 16;
                break;
            case AV_SAMPLE_FMT_S32:       ///< signed 32 bits
                this->sampleSize = 32;
            default:
                break;
            }
            printf("audio sample rate:%d sample size:%d chanle:%d\n",
                this->sampleRate, this->sampleSize, this->channel);

        }
    }
    return totalMs;
}

void FFmpegLogic::CloseFile()
{
    QMutexLocker m_lock(&mutexFFmpeg);
    if (ic)
    {
        // 该函数用于关闭一个AVFormatContext，一般情况下是和avformat_open_input()成对使用的。
        avformat_close_input(&ic);
    }
    if (yuv)
    {
        // 释放AVFrame存储解码后的原始音频或视频数据。
        av_frame_free(&yuv);
    }

    if (cCtx)
    {
        sws_freeContext(cCtx);
        cCtx = NULL;
    }
    if (aCtx)
    {
        swr_free(&aCtx);
    }
}
