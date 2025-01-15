#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "MainMenuWidget.h"
#include "ImageDisplayWidget.h"
#include "SVGRenderWorker.h"
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;


private:
    MainMenuWidget *menuWidget;
    ImageDisplayWidget *imageWidget;

    QThread *svgRenderThread;
    SVGRenderWorker *svgRenderWorker;

    void adjustImageDisplaySize();
    void adjustMenuSize();
    void setupConnections();

};

#endif // MAINWINDOW_H
