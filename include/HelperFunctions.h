//
// Created by Decmo on 25-1-16.
//

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <glm/glm.hpp>
#include "SVGNode.h"
#include <sstream>
#include <functional>
glm::mat3 scale3x3(float scaleX, float scaleY);
glm::mat3 getScale(const SVGNode* root);
std::vector<float> parseNumbers(const std::string& str);
void computeTransform(SVGNode* root);
#endif //HELPERFUNCTIONS_H
