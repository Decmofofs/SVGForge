#include "XMLParser.h"
#include <QFile>
#include <QDebug>

XMLParser::XMLParser(const QString& filePath)
    : filePath(filePath) {}

SVGNode* XMLParser::parse() {
    QFile file(filePath);

    // 打开文件并检查是否成功
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return nullptr;
    }

    // 将文件内容加载到 QXmlStreamReader
    xmlReader.setDevice(&file);

    // 查找根元素
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            return parseElement(); // 从根元素开始解析
        }
    }

    // 检查解析错误
    if (xmlReader.hasError()) {
        qWarning() << "XML Parsing Error:" << xmlReader.errorString();
    }

    return nullptr; // 如果没有找到根元素或出现错误
}

SVGNode* XMLParser::parseElement() {
    if (xmlReader.tokenType() != QXmlStreamReader::StartElement) {
        return nullptr;
    }

    // 创建节点并设置类型
    SVGNode* node = new SVGNode(mapToSVGElement(xmlReader.name().toString()));

    // 解析属性
    parseAttributes(node);

    // 递归解析子元素
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartElement) {
            SVGNode* childNode = parseElement(); // 解析子节点
            if (childNode) {
                node->addChild(childNode);
            }
        } else if (token == QXmlStreamReader::EndElement) {
            break; // 当前元素结束
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
