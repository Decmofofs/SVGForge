#include "SVGRenderWorker.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QImage>

SVGRenderWorker::SVGRenderWorker(QObject *parent)
    : QObject(parent)

{

}

void SVGRenderWorker::renderSVG(const QString &filePath)
{
    // 这里可以异步调用 renderSVGFile 方法
    QPixmap pixmap = renderSVGFile(filePath);

    // 渲染完成后，发出信号
    emit renderFinished(pixmap);
}

QPixmap SVGRenderWorker::renderSVGFile(const QString &filePath)
{
    // 这是具体渲染的占位实现
    QSize targetSize(400, 300); // 假设渲染目标尺寸
    QPixmap pixmap(targetSize);
    pixmap.fill(Qt::black);

    // 使用 QSvgRenderer 渲染 SVG
    //QSvgRenderer svgRenderer(filePath);
    // if (svgRenderer.isValid()) {
    //     QPainter painter(&pixmap);
    //     svgRenderer.render(&painter);
    // } else {
    //     // 渲染失败时返回一个错误图像或日志
    //     pixmap.fill(Qt::red);
    // }

    QSvgRenderer svgRenderer(filePath);
    pixmap = QPixmap(svgRenderer.defaultSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing); // 文本抗锯齿

    svgRenderer.render(&painter);

    return pixmap;
}
