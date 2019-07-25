#pragma once

/***************************** �ļ���Ϣ ***********************************
**
** Project       : FFmpegLogic
** Description   : FFMPEG�����װ��
** Creation date : 2019��7��25��14:50:40
**
*******************************************************************************/

#if _MSC_VER > 1600
#pragma  execution_character_set("utf-8")
#endif

#include "QMutex"
#include "QMutexLocker"

// ����FFmpegͷ�ļ�
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libavutil/time.h>
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

class FFmpegLogic
{
private:
    FFmpegLogic();
    ~FFmpegLogic();
public:
    static FFmpegLogic *GetInstance();
    static void Uninitialize();
private:
    static FFmpegLogic *m_pInstance;
    void InitBasicData();
public:
    //************************************
    //            ����Ƶ�ļ�
    // Method:    OpenFile
    // FullName:  FFmpegLogic::OpenFile
    // Access:    public 
    // Returns:   int ��Ƶʱ��
    // Qualifier:
    // Parameter: const char * path �ļ�·��
    //************************************
    int OpenFile(const char *path);

    //************************************
    //            �ر��Ѿ��򿪵��ļ�
    // Method:    CloseFile
    // FullName:  FFmpegLogic::CloseFile
    // Access:    public 
    // Returns:   void
    // Qualifier:
    //************************************
    void CloseFile();


private:
    QMutex mutexFFmpeg;
    char errorbuf[1024];
    AVFormatContext *ic = NULL; 
    AVFrame *yuv = NULL; // ͼ������
    AVFrame *pcm = NULL; // ��Ƶ����
    SwsContext *cCtx = NULL;
    SwrContext *aCtx = NULL;

    int totalMs = 0; // ��ʱ��
    int fps = 0;
    int pts = 0;
    int videoStream = 0;    // ��Ƶ��
    int audioStream = 1;    // ��Ƶ��
    int sampleRate = 48000; // ������
    int sampleSize = 16;    // ����λ��
    int channel = 2;        // ������
    bool isPlay = false;    // �Ƿ񲥷�

};
