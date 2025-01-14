#ifndef IMAGEDISPLAYWIDGET_H
#define IMAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

class ImageDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageDisplayWidget(QWidget *parent = nullptr);
    void setImage(const QPixmap &pixmap);

private:
    QLabel *imageLabel; // 用于显示图片的标签
};

#endif // IMAGEDISPLAYWIDGET_H

