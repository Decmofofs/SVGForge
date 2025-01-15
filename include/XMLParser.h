#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QString>
#include "SVGNode.h"
#include <QXmlStreamReader>

class XMLParser {
public:
    // 构造函数，接受文件路径
    explicit XMLParser(const QString& filePath);

    // 解析 SVG 文件并返回根节点
    SVGNode* parse();

private:
    QXmlStreamReader xmlReader; // Qt 的 XML 流解析器
    QString filePath;          // 文件路径

    // 递归解析单个元素
    SVGNode* parseElement();

    // 将标签名映射到 SVGElements 枚举
    SVGElements mapToSVGElement(const QString& tagName);

    // 检查并提取属性
    void parseAttributes(SVGNode* node);
};

#endif // XMLPARSER_H
