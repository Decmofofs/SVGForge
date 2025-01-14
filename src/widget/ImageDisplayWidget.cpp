#include "ImageDisplayWidget.h"

ImageDisplayWidget::ImageDisplayWidget(QWidget *parent)
    : QWidget(parent)
{
    // 初始化 QLabel，用于显示图片
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background-color: white; border: 1px solid gray;");

    // 使用布局管理器将 QLabel 填充到整个窗口
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);
    setLayout(layout);
}

void ImageDisplayWidget::setImage(const QPixmap &pixmap)
{
    //imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setPixmap(pixmap);
}
