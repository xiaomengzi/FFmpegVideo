#include "FFmpegVideo.h"

FFmpegVideo::FFmpegVideo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    /********************** ���沼�� *******************************/
    // [2019-7-25 megnxw] �����Ƶ��ʾ����
    VideoDisplayWidget* pVideoWidget = new VideoDisplayWidget(this);
    ui.videoLayout->addWidget(pVideoWidget);

    // �����ļ��б�
    ui.fileListWidget->setHidden(true);

    // ���ð�ť״̬
    ui.btnPlay->setEnabled(false);
    ui.btnPause->setEnabled(false);
    ui.btnStop->setEnabled(false);

    /********************** ���źŲ� *******************************/
    connect(ui.btnOpenFile, SIGNAL(clicked()), this, SLOT(slt_btnOpenFile_clicked()));
    connect(ui.btnPlay, SIGNAL(clicked()), this, SLOT(slt_btnPlay_clicked()));
    connect(ui.btnPause, SIGNAL(clicked()), this, SLOT(slt_btnPause_clicked()));
    connect(ui.btnStop, SIGNAL(clicked()), this, SLOT(slt_btnStop_clicked()));
    connect(ui.btnFileList, SIGNAL(clicked()), this, SLOT(slt_btnFileList_clicked()));
}

void FFmpegVideo::slt_btnOpenFile_clicked()
{
    QString strDlgTitle = tr("����Ƶ�ļ�");
    QString strHomePath = QDir::homePath();
    QString strFilter = tr("��Ƶ�ļ�(*.flv *.rmvb *.avi *.MP4);; �����ļ�(*.*)");

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
        // ������Ƶ�ļ���ʱ��
        SetVideoFileTotalTime(totalMs);
        // ���ò��ſ��ư�ť״̬
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
    // ������Ƶ��ʱ����
    QString strTotalTime = "";
    int hour = (nTotalTime / 1000) / 60 / 60; // Сʱ
    int min = (nTotalTime / 1000) / 60; // ����
    int sec = (nTotalTime / 1000) % 60;// ��

    // ��ʽ��ʱ���ַ���
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

