#include "MainWindow.h"
#include <QResizeEvent>
#include <QShowEvent>
#include <QThread>

#include "GlobalData.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      svgRenderThread(new QThread),
      svgRenderWorker(new SVGRenderWorker) {

    menuWidget = new MainMenuWidget(this);
    imageWidget = new ImageDisplayWidget(this);

    setMinimumSize(1000, 800);

    svgRenderWorker->moveToThread(svgRenderThread);

    setWindowTitle("SVGForge v0.1");
    setupConnections();
    svgRenderThread->start();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    adjustImageDisplaySize();
    adjustMenuSize();
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    adjustImageDisplaySize();
    adjustMenuSize();
}

void MainWindow::adjustImageDisplaySize() {

    int rightWidth = static_cast<int>(this->width()*0.75);
    int rightHeight = static_cast<int>(this->height());

    int displayWidth = static_cast<int>(rightWidth * 0.8);
    int displayHeight = static_cast<int>(rightHeight * 0.8);

    imageWidget->setFixedSize(displayWidth, displayHeight);

    imageWidget->move(static_cast<int>(rightWidth*0.1+this->width()*0.25), static_cast<int>(rightHeight*0.1));
}

void MainWindow::adjustMenuSize() {

    int leftWidth = static_cast<int>(this->width()*0.325);
    int leftHeight = static_cast<int>(this->height());

    int displayWidth = static_cast<int>(leftWidth * 0.8);
    int displayHeight = static_cast<int>(leftHeight * 0.8);

    menuWidget->setFixedSize(displayWidth, displayHeight);
    menuWidget->move(static_cast<int>(leftWidth*0.1),static_cast<int>(leftHeight*0.1));
}

void MainWindow::setupConnections() {
    // 菜单选择文件信号连接到渲染 Worker 的槽
    connect(menuWidget, &MainMenuWidget::fileSelected, svgRenderWorker, &SVGRenderWorker::renderSVG);

    // 渲染完成信号连接到界面更新槽
    connect(svgRenderWorker, &SVGRenderWorker::renderFinished, this, [this](const QPixmap &pixmap) {
        imageWidget->setImage(pixmap);
        *globalPix = pixmap;
        renderFinished = 1;
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

MainWindow::~MainWindow() {
    svgRenderThread->quit();
    svgRenderThread->wait();
    delete svgRenderThread;
    delete svgRenderWorker;
}

