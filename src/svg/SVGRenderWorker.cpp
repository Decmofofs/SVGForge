#include "SVGRenderWorker.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QImage>
#include "SVGNode.h"
#include "XMLParser.h"
#include "GlobalData.h"
#include "HelperFunctions.h"
#include "SVGRenderer.h"
#include <QDebug>
SVGRenderWorker::SVGRenderWorker(QObject *parent)
    : QObject(parent) {

}

void SVGRenderWorker::renderSVG(const QString &filePath) {

    QPixmap pixmap = renderSVGFile(filePath);

    emit renderFinished(pixmap);
}

QPixmap SVGRenderWorker::renderSVGFile(const QString &filePath) {
    QPixmap pixmap;
    pixmap.fill(Qt::black);



    XMLParser parser(filePath);

    SVGNode *root = parser.parse();

    globalTransformMatrix = getScale(root);

    auto userScale = scale3x3(scale, scale);
    globalTransformMatrix = globalTransformMatrix * userScale;

    computeTransform(root);
    applyGlobalTransform(root, globalTransformMatrix);

    auto [width, height] = getWidthAndHeight(root);
    width = width * scale;
    height = height * scale;

    std::vector<std::vector<glm::vec4> > renderBuffer(width, std::vector<glm::vec4>(height, glm::vec4(0.0f)));

    SVGRenderer(renderBuffer, root, width, height);

    vectorToPixmap(renderBuffer, pixmap);

    return pixmap;
}
