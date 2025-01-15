#ifndef SVGNODE_H
#define SVGNODE_H
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include "SVGElements.h"

class SVGNode {
public:

    SVGNode(SVGElements type = SVGElements::Unknown);
    ~SVGNode();

    void setAttribute(const std::string& name, const std::string& value);
    std::string getAttribute(const std::string& name) const;

    void addChild(SVGNode* child);
    const std::vector<SVGNode*>& getChildren() const;

    SVGElements getType() const;

    void setTransformMatrix(const glm::mat3& matrix);
    glm::mat3 getTransformMatrix() const;

private:
    SVGElements type;
    std::map<std::string, std::string> attributes;
    std::vector<SVGNode*> children;

    glm::mat3 transformMatrix;
};

#endif // SVGNODE_H
