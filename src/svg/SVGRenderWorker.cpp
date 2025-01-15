#include "SVGRenderWorker.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QImage>
#include "SVGNode.h"
#include "XMLParser.h"
#include "GlobalData.h"
#include "HelperFunctions.h"

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
    QPixmap pixmap;
    pixmap.fill(Qt::black);



    XMLParser parser(filePath);

    SVGNode *root = parser.parse();

    globalTransformMatrix = getScale(root);
    auto userScale = scale3x3(scale, scale);
    globalTransformMatrix = globalTransformMatrix * userScale;

    computeTransform(root);


    return pixmap;
}
