#include "MainWindow.h"
#include <QResizeEvent>
#include <QShowEvent>
#include <QThread>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      svgRenderThread(new QThread),
      svgRenderWorker(new SVGRenderWorker)
{
    // 创建主窗口中央组件
    auto *mainWidget = new QWidget(this);

    // 初始化菜单和图片显示区域
    menuWidget = new MainMenuWidget(this);
    imageWidget = new ImageDisplayWidget(this);

    // 用于限制右侧区域比例的容器
    imageContainer = new QWidget(this);
    auto *imageContainerLayout = new QVBoxLayout(imageContainer);
    imageContainerLayout->addWidget(imageWidget, 0, Qt::AlignCenter);
    imageContainerLayout->setContentsMargins(0, 0, 0, 0);

    // 设置主布局
    auto *mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->addWidget(menuWidget, 1);        // 左侧菜单区域占比1
    mainLayout->addWidget(imageContainer, 3);   // 右侧图片区域占比3
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 设置主窗口的中央组件
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    // 设置主窗口的最小尺寸
    setMinimumSize(1000, 800);

    svgRenderWorker->moveToThread(svgRenderThread);

    setWindowTitle("SVGForge v0.1");
    setupConnections();
    svgRenderThread->start();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // 调整图片显示区域大小
    adjustImageDisplaySize();
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // 调整图片显示区域大小
    adjustImageDisplaySize();
}

void MainWindow::adjustImageDisplaySize()
{
    // 获取右侧区域的大小
    int rightWidth = imageContainer->width();
    int rightHeight = imageContainer->height();

    // 计算 ImageDisplayWidget 的宽度和高度（80%）
    int displayWidth = static_cast<int>(rightWidth * 0.8);
    int displayHeight = static_cast<int>(rightHeight * 0.8);

    // 设置 ImageDisplayWidget 的固定大小
    imageWidget->setFixedSize(displayWidth, displayHeight);
}

void MainWindow::setupConnections()
{
    // 菜单选择文件信号连接到渲染 Worker 的槽
    connect(menuWidget, &MainMenuWidget::fileSelected, svgRenderWorker, &SVGRenderWorker::renderSVG);

    // 渲染完成信号连接到界面更新槽
    connect(svgRenderWorker, &SVGRenderWorker::renderFinished, this, [this](const QPixmap &pixmap) {
        imageWidget->setImage(pixmap);
    });

    // 渲染线程启动日志（可选）
    connect(svgRenderThread, &QThread::started, []() {
        qDebug() << "Render thread started.";
    });

    // 渲染线程结束信号连接到清理（可选）
    connect(svgRenderThread, &QThread::finished, []() {
        qDebug() << "Render thread finished.";
    });
}
