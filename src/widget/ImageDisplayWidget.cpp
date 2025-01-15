#include "ImageDisplayWidget.h"

ImageDisplayWidget::ImageDisplayWidget(QWidget *parent)
    : QWidget(parent) {

    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background-color: white; border: 1px solid gray;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);
    setLayout(layout);
}

void ImageDisplayWidget::setImage(const QPixmap &pixmap) {
    //imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setPixmap(pixmap);
}
