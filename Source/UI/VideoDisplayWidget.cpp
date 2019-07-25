#include "VideoDisplayWidget.h"

VideoDisplayWidget::VideoDisplayWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

VideoDisplayWidget::~VideoDisplayWidget()
{
}


void VideoDisplayWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色


    if (m_image.size().width() <= 0)
    {
        // 图片错误，直接返回
        return;
    }

    // 将图像按比例缩放成和窗口一样大小
    QImage img = m_image.scaled(this->size(), Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x, y), img); //画出图像
}

void VideoDisplayWidget::slot_GetOneFrame(QImage img)
{
    m_image = img;
    update(); //调用update将执行 paintEvent函数
}
