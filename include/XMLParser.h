#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QString>
#include "SVGNode.h"
#include <QXmlStreamReader>

class XMLParser {
public:

    explicit XMLParser(const QString& filePath);

    SVGNode* parse();

private:
    QXmlStreamReader xmlReader;
    QString filePath;

    SVGNode* parseElement();

    SVGElements mapToSVGElement(const QString& tagName);

    void parseAttributes(SVGNode* node);
};

#endif // XMLPARSER_H
