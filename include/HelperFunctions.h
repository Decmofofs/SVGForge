//
// Created by Decmo on 25-1-16.
//

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <glm/glm.hpp>
#include "SVGNode.h"
#include <sstream>
#include <functional>
#include <QPixmap>

glm::mat3 scale3x3(float scaleX, float scaleY);
glm::mat3 getScale(const SVGNode* root);
std::vector<float> parseNumbers(const std::string& str);
void computeTransform(SVGNode* root);
std::pair<float,float> getWidthAndHeight(const SVGNode* root);
void applyGlobalTransform(SVGNode* node, const glm::mat3& globalTransformMatrix);
void vectorToPixmap(std::vector<std::vector<glm::vec4> > &renderBuffer, QPixmap &pixmap);
int isInPolygon(float x, float y, const std::vector<glm::vec2>& poly);

#endif //HELPERFUNCTIONS_H
