#include "XMLParser.h"
#include <QFile>
#include <QDebug>

XMLParser::XMLParser(const QString& filePath)
    : filePath(filePath) {}

SVGNode* XMLParser::parse() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return nullptr;
    }
    xmlReader.setDevice(&file);
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            return parseElement();
        }
    }
    if (xmlReader.hasError()) {
        qWarning() << "XML Parsing Error:" << xmlReader.errorString();
    }
    return nullptr;
}

SVGNode* XMLParser::parseElement() {
    if (xmlReader.tokenType() != QXmlStreamReader::StartElement) {
        return nullptr;
    }
    SVGNode* node = new SVGNode(mapToSVGElement(xmlReader.name().toString()));
    parseAttributes(node);
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            SVGNode* childNode = parseElement();
            if (childNode) {
                node->addChild(childNode);
            }
        } else if (token == QXmlStreamReader::EndElement) {
            break;
        }
    }
    return node;
}

void XMLParser::parseAttributes(SVGNode* node) {
    foreach (const QXmlStreamAttribute& attr, xmlReader.attributes()) {
        node->setAttribute(attr.name().toString().toStdString(), attr.value().toString().toStdString());
    }
}

SVGElements XMLParser::mapToSVGElement(const QString& tagName) {
    if (tagName == "circle") return SVGElements::Circle;
    if (tagName == "rect") return SVGElements::Rect;
    if (tagName == "line") return SVGElements::Line;
    if (tagName == "ellipse") return SVGElements::Ellipse;
    if (tagName == "polygon") return SVGElements::Polygon;
    if (tagName == "polyline") return SVGElements::Polyline;
    if (tagName == "path") return SVGElements::Path;
    if (tagName == "text") return SVGElements::Text;
    if (tagName == "g") return SVGElements::G;
    if (tagName == "svg") return SVGElements::Svg;
    return SVGElements::Unknown;
}
