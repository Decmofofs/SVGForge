#include "SVGNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>



SVGNode::SVGNode(SVGElements type)
    : type(type),transformMatrix(glm::mat3(1.0f)) {}

SVGNode::~SVGNode() {
    for (SVGNode* child : children) {
        delete child;
    }
}

void SVGNode::setAttribute(const std::string& name, const std::string& value) {
    attributes[name] = value;
}

std::string SVGNode::getAttribute(const std::string& name) const {
    auto it = attributes.find(name);
    return (it != attributes.end()) ? it->second : "";
}

void SVGNode::addChild(SVGNode* child) {
    if (child) {
        children.push_back(child);
    }
}

const std::vector<SVGNode*>& SVGNode::getChildren() const {
    return children;
}

SVGElements SVGNode::getType() const {
    return type;
}

void SVGNode::setTransformMatrix(const glm::mat3& matrix) {
    transformMatrix = matrix;
}

glm::mat3 SVGNode::getTransformMatrix() const {
    return transformMatrix;
}
