#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FFmpegVideo.h"

class FFmpegVideo : public QMainWindow
{
    Q_OBJECT

public:
    FFmpegVideo(QWidget *parent = Q_NULLPTR);

private:
    Ui::FFmpegVideoClass ui;
};
