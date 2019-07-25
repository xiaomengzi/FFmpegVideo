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
    painter.drawRect(0, 0, this->width(), this->height()); //�Ȼ��ɺ�ɫ


    if (m_image.size().width() <= 0)
    {
        // ͼƬ����ֱ�ӷ���
        return;
    }

    // ��ͼ�񰴱������ųɺʹ���һ����С
    QImage img = m_image.scaled(this->size(), Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x, y), img); //����ͼ��
}

void VideoDisplayWidget::slot_GetOneFrame(QImage img)
{
    m_image = img;
    update(); //����update��ִ�� paintEvent����
}
