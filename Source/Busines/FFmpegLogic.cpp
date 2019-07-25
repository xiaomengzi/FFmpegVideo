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
    // ע�����еĸ�ʽ���������װ��ʽ�ͼӷ�װ��ʽ��
    av_register_all();
    // ���ڳ�ʼ�����硣FFmpeg����Ҳ֧�ֽ��װRTSP������, ���Ҫ���װ�������ݸ�ʽ����ɵ��øú�����
    avformat_network_init();
}

int FFmpegLogic::OpenFile(const char *path)
{
    CloseFile();
    QMutexLocker m_lock(&mutexFFmpeg);
    // ��ʼ�� AVFormatContext
    ic = avformat_alloc_context();

    // ��һ���ļ����������ɽ��������ݰ�������Ƶ������Ƶ������Ƶ����������Ƶ����������Ƶ֡������
    int re = avformat_open_input(&ic, path, 0, 0);
    if (re != 0)
    {
        // ��ȡ������Ϣ
        av_strerror(re, errorbuf, sizeof(errorbuf));
        printf("open %s failed: %s\n", path, errorbuf);
        return 0;
    }
    // ��ȡ�ļ�ʱ������λ(ms)
    totalMs = ((ic->duration / AV_TIME_BASE) * 1000);

    // ������Ƶ�� [remark ???]
    for (int i = 0; i < ic->nb_streams; i++)
    {
        AVCodecContext *enc = ic->streams[i]->codec;

        if (enc->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            fps = r2d(ic->streams[i]->avg_frame_rate);
            // ��ȡ������ 
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
        // �ú������ڹر�һ��AVFormatContext��һ��������Ǻ�avformat_open_input()�ɶ�ʹ�õġ�
        avformat_close_input(&ic);
    }
    if (yuv)
    {
        // �ͷ�AVFrame�洢������ԭʼ��Ƶ����Ƶ���ݡ�
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
