#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "MainMenuWidget.h"
#include "ImageDisplayWidget.h"
#include "SVGRenderWorker.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override; // 重写 resizeEvent
    void showEvent(QShowEvent *event) override;     // 重写 showEvent

private:
    MainMenuWidget *menuWidget;         // 菜单区域
    ImageDisplayWidget *imageWidget;   // 显示图片区域
    QWidget *imageContainer;           // 用于限制右侧区域比例的容器

    QThread *svgRenderThread;          // SVG 渲染线程
    SVGRenderWorker *svgRenderWorker;  // SVG 渲染工作线程

    void adjustImageDisplaySize();     // 调整 ImageDisplayWidget 的大小
    void setupConnections();           // 设置信号槽连接
};

#endif // MAINWINDOW_H
