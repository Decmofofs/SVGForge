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
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override; // 重写 resizeEvent
    void showEvent(QShowEvent *event) override;     // 重写 showEvent


private:
    MainMenuWidget *menuWidget;         // 菜单区域
    ImageDisplayWidget *imageWidget;   // 显示图片区域

    QThread *svgRenderThread;          // SVG 渲染线程
    SVGRenderWorker *svgRenderWorker;  // SVG 渲染工作线程

    void adjustImageDisplaySize();     // 调整 ImageDisplayWidget 的大小
    void adjustMenuSize();             // 调整 MainMenuWidget 的大小
    void setupConnections();           // 设置信号槽连接

};

#endif // MAINWINDOW_H
