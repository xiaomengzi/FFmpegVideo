#pragma once

/***************************** 文件信息 ***********************************
**
** Project       : FFmpegLogic
** Description   : FFMPEG处理封装类
** Creation date : 2019年7月25日14:50:40
**
*******************************************************************************/

#if _MSC_VER > 1600
#pragma  execution_character_set("utf-8")
#endif

#include "QMutex"
#include "QMutexLocker"

// 导入FFmpeg头文件
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
    //            打开视频文件
    // Method:    OpenFile
    // FullName:  FFmpegLogic::OpenFile
    // Access:    public 
    // Returns:   int 视频时长
    // Qualifier:
    // Parameter: const char * path 文件路径
    //************************************
    int OpenFile(const char *path);

    //************************************
    //            关闭已经打开的文件
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
    AVFrame *yuv = NULL; // 图像数据
    AVFrame *pcm = NULL; // 音频数据
    SwsContext *cCtx = NULL;
    SwrContext *aCtx = NULL;

    int totalMs = 0; // 总时长
    int fps = 0;
    int pts = 0;
    int videoStream = 0;    // 视频流
    int audioStream = 1;    // 音频流
    int sampleRate = 48000; // 采样率
    int sampleSize = 16;    // 采样位数
    int channel = 2;        // 声道数
    bool isPlay = false;    // 是否播放

};
