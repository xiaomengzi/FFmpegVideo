#pragma once

#if _MSC_VER > 1600
#pragma  execution_character_set("utf-8")
#endif

#include <QtWidgets/QMainWindow>
#include "ui_FFmpegVideo.h"
#include "QMessageBox"
#include "QFileDialog"
#include "QDir"

#include "VideoDisplayWidget.h"
#include "FFmpegLogic.h"


class FFmpegVideo : public QMainWindow
{
    Q_OBJECT

public:
    FFmpegVideo(QWidget *parent = Q_NULLPTR);

    private slots:
    void slt_btnOpenFile_clicked();
    void slt_btnPlay_clicked();
    void slt_btnPause_clicked();
    void slt_btnStop_clicked();
    void slt_btnFileList_clicked();

private:
    //************************************
    // Method:    SetVideoFileTotalTime
    // FullName:  FFmpegVideo::SetVideoFileTotalTime
    // Access:    private 
    // Returns:   void
    // Qualifier:
    // Parameter: int nTotalTime-视频总时长
    //************************************
    void SetVideoFileTotalTime(int nTotalTime);
private:
    Ui::FFmpegVideoClass ui;

};
