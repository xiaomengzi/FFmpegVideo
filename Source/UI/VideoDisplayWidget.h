#pragma once

#if _MSC_VER > 1600
#pragma  execution_character_set("utf-8")
#endif

#include <QWidget>
#include "ui_VideoDisplayWidget.h"
#include "QPaintEvent"
#include "QPainter"
#include "QImage"

class VideoDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    VideoDisplayWidget(QWidget *parent = Q_NULLPTR);
    ~VideoDisplayWidget();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    // 播放器相关的槽函数
    void slot_GetOneFrame(QImage img);

private:
    Ui::VideoDisplayWidget ui;

protected:
    QImage m_image; //记录当前的图像
};
