#include "FFmpegVideo.h"

FFmpegVideo::FFmpegVideo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    /********************** 界面布局 *******************************/
    // [2019-7-25 megnxw] 添加视频显示窗体
    VideoDisplayWidget* pVideoWidget = new VideoDisplayWidget(this);
    ui.videoLayout->addWidget(pVideoWidget);

    // 隐藏文件列表
    ui.fileListWidget->setHidden(true);

    // 设置按钮状态
    ui.btnPlay->setEnabled(false);
    ui.btnPause->setEnabled(false);
    ui.btnStop->setEnabled(false);

    /********************** 绑定信号槽 *******************************/
    connect(ui.btnOpenFile, SIGNAL(clicked()), this, SLOT(slt_btnOpenFile_clicked()));
    connect(ui.btnPlay, SIGNAL(clicked()), this, SLOT(slt_btnPlay_clicked()));
    connect(ui.btnPause, SIGNAL(clicked()), this, SLOT(slt_btnPause_clicked()));
    connect(ui.btnStop, SIGNAL(clicked()), this, SLOT(slt_btnStop_clicked()));
    connect(ui.btnFileList, SIGNAL(clicked()), this, SLOT(slt_btnFileList_clicked()));
}

void FFmpegVideo::slt_btnOpenFile_clicked()
{
    QString strDlgTitle = tr("打开视频文件");
    QString strHomePath = QDir::homePath();
    QString strFilter = tr("视频文件(*.flv *.rmvb *.avi *.MP4);; 所有文件(*.*)");

    QString strFilePath = QFileDialog::getOpenFileName(this, strDlgTitle, strHomePath, strFilter);
    if (!strFilePath.isEmpty())
    {
        strFilePath.replace("/", "\\");
        this->setWindowTitle(strFilePath);
        int totalMs = FFmpegLogic::GetInstance()->OpenFile(strFilePath.toLocal8Bit());
        if (totalMs <= 0)
        {
            QMessageBox::information(this, "err", "file open failed!");
            return;
        }
        // 设置视频文件总时长
        SetVideoFileTotalTime(totalMs);
        // 设置播放控制按钮状态
    }
}

void FFmpegVideo::slt_btnPlay_clicked()
{

}

void FFmpegVideo::slt_btnPause_clicked()
{

}

void FFmpegVideo::slt_btnStop_clicked()
{

}

void FFmpegVideo::slt_btnFileList_clicked()
{
    ui.fileListWidget->isVisible();
    if ( false == ui.fileListWidget->isVisible())
    {
        ui.fileListWidget->setHidden(false);
    }
    else
    {
        ui.fileListWidget->setHidden(true);
    }
}

void FFmpegVideo::SetVideoFileTotalTime(int nTotalTime)
{
    // 计算视频的时分秒
    QString strTotalTime = "";
    int hour = (nTotalTime / 1000) / 60 / 60; // 小时
    int min = (nTotalTime / 1000) / 60; // 分钟
    int sec = (nTotalTime / 1000) % 60;// 秒

    // 格式化时长字符串
    if ( 0 < hour)
    {
        strTotalTime = QString::asprintf("%02d:%02d:%02d", hour, min, sec);
    }
    else
    {
        strTotalTime = QString::asprintf("%02d:%02d", min, sec);
    }
    ui.labelTotalTime->setText(strTotalTime);
}

