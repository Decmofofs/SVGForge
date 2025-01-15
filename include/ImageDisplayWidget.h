#ifndef IMAGEDISPLAYWIDGET_H
#define IMAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

class ImageDisplayWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImageDisplayWidget(QWidget *parent = nullptr);
    void setImage(const QPixmap &pixmap);

private:
    QLabel *imageLabel;
};

#endif // IMAGEDISPLAYWIDGET_H

