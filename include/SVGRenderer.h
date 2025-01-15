//
// Created by Decmo on 25-1-16.
//

#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include <vector>
#include "SVGNode.h"
#include "HelperFunctions.h"

void SVGRenderer(std::vector<std::vector<glm::vec4> > &renderBuffer, SVGNode *root, int rWidth, int rHeight);
#endif //SVGRENDERER_H
