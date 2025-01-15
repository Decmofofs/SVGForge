#include "SVGRenderer.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <QDebug>
#include "GlobalData.h"
#include <PreDefinedColors.h>
#include <vector>
inline void fillPixel(std::vector<std::vector<glm::vec4>>& renderBuffer,
                      int x, int y,
                      const glm::vec4& color) {
    int rWidth = static_cast<int>(renderBuffer.size());
    if (rWidth == 0) return;
    int rHeight = static_cast<int>(renderBuffer[0].size());

    if (x >= 0 && x < rWidth && y >= 0 && y < rHeight) {
        renderBuffer[x][y] = color;
    }
}

inline float getFloatAttribute(const SVGNode* node, const std::string& name, float defaultVal = 0.0f) {
    std::string valStr = node->getAttribute(name);
    if (valStr.empty()) {
        return defaultVal;
    }
    return std::stof(valStr);
}

inline glm::vec4 getFillColor(const SVGNode* node) {

    std::string fillStr = node->getAttribute("fill");
    if (fillStr.empty() || fillStr == "none") {
        return glm::vec4(0.0f);
    }

    if (fillStr[0] == '#') {
        // "#RRGGBB"
        if (fillStr.size() == 7) {
            int r = std::stoi(fillStr.substr(1,2), nullptr, 16);
            int g = std::stoi(fillStr.substr(3,2), nullptr, 16);
            int b = std::stoi(fillStr.substr(5,2), nullptr, 16);
            return glm::vec4(r/255.f, g/255.f, b/255.f, 1.0f);
        }
    }

    if (svgColorTable.find(fillStr) != svgColorTable.end()) {
        float r = svgColorTable[fillStr].r;
        float g = svgColorTable[fillStr].g;
        float b = svgColorTable[fillStr].b;
        float a = svgColorTable[fillStr].a;
        return glm::vec4(r,g,b,a);
    }

    return glm::vec4({0,0,0,0});
}

inline glm::vec4 getStrokeColor(const SVGNode * node) {
    std::string strokeStr = node->getAttribute("stroke");
    if (strokeStr.empty() || strokeStr == "none") {
        return glm::vec4(0.0f);
    }
    if (strokeStr[0] == '#') {
        // "#RRGGBB"
        if (strokeStr.size() == 7) {
            int r = std::stoi(strokeStr.substr(1,2), nullptr, 16);
            int g = std::stoi(strokeStr.substr(3,2), nullptr, 16);
            int b = std::stoi(strokeStr.substr(5,2), nullptr, 16);
            return glm::vec4(r/255.f, g/255.f, b/255.f, 1.0f);
        }
    }

    if (svgColorTable.find(strokeStr) != svgColorTable.end()) {
        float r = svgColorTable[strokeStr].r;
        float g = svgColorTable[strokeStr].g;
        float b = svgColorTable[strokeStr].b;
        float a = svgColorTable[strokeStr].a;
        return glm::vec4(r,g,b,a);
    }

    return glm::vec4({0,0,0,0});
}


void drawRect(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight) {
    float x = getFloatAttribute(node, "x");
    float y = getFloatAttribute(node, "y");
    float w = getFloatAttribute(node, "width");
    float h = getFloatAttribute(node, "height");

    glm::vec4 color = getFillColor(node);
    qInfo() << x << y << w << h;
    if (color.a == 0.0f) {

        return;
    }

    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int x1 = static_cast<int>(std::ceil(x + w));
    int y1 = static_cast<int>(std::ceil(y + h));

    glm::vec2 topLeft = transform * glm::vec3{x0, y0, 1.0f};
    glm::vec2 topRight = transform * glm::vec3{x1, y0, 1.0f};
    glm::vec2 bottomLeft = transform * glm::vec3{x0, y1, 1.0f};
    glm::vec2 bottomRight = transform * glm::vec3{x1, y1, 1.0f};

    float minX = std::min({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
    float maxX = std::max({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
    float minY = std::min({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
    float maxY = std::max({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});

    qInfo() << minX << maxX << minY << maxY << "FEAFEF";

    for (int px = minX; px < maxX; ++px) {
        for (int py = minY; py < maxY; ++py) {

            if (!isInPolygon(px,py, {topLeft, topRight, bottomRight, bottomLeft})) {
                continue;
            }

            glm::vec3 localPos(px, py, 1.0f);
            glm::vec3 worldPos = localPos;

            int finalX = static_cast<int>(std::round(worldPos.x));
            int finalY = static_cast<int>(std::round(worldPos.y));

            fillPixel(renderBuffer, finalX, finalY, color);
        }
    }
}

void drawCircle(const SVGNode* node,
                std::vector<std::vector<glm::vec4>>& renderBuffer,
                const glm::mat3& transform,
                int rWidth,
                int rHeight) {
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    float r  = getFloatAttribute(node, "r");

    glm::vec4 color = getFillColor(node);
    if (color.a == 0.0f) {
        return;
    }

    glm::vec3 newC = transform * glm::vec3(cx, cy, 1.0f);
    glm::vec3 radiusPoint = transform * glm::vec3(cx + r, cy, 1.0f);
    float newR = glm::distance(newC, radiusPoint);
    float newCx = newC.x;
    float newCy = newC.y;

    int minX = static_cast<int>(std::floor(newCx - newR));
    int maxX = static_cast<int>(std::ceil(newCx + newR));
    int minY = static_cast<int>(std::floor(newCy - newR));
    int maxY = static_cast<int>(std::ceil(  newCy + newR));

    for (int px = minX; px <= maxX; ++px) {
        for (int py = minY; py <= maxY; ++py) {

            float dx = px - newCx;
            float dy = py - newCy;
            if (dx*dx + dy*dy <= newR*newR) {

                glm::vec3 localPos(px, py, 1.0f);
                glm::vec3 worldPos = localPos;
                int finalX = static_cast<int>(std::round(worldPos.x));
                int finalY = static_cast<int>(std::round(worldPos.y));
                fillPixel(renderBuffer, finalX, finalY, color);
            }
        }
    }
}

void drawLine(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight) {
    float x1 = getFloatAttribute(node, "x1");
    float y1 = getFloatAttribute(node, "y1");
    float x2 = getFloatAttribute(node, "x2");
    float y2 = getFloatAttribute(node, "y2");


    std::string stroke = node->getAttribute("stroke");
    glm::vec4 color = getStrokeColor(node);

    glm::vec3 pt1 = transform * glm::vec3(x1, y1, 1.0f);
    glm::vec3 pt2 = transform * glm::vec3(x2, y2, 1.0f);

    int ix1 = static_cast<int>(std::round(pt1.x));
    int iy1 = static_cast<int>(std::round(pt1.y));
    int ix2 = static_cast<int>(std::round(pt2.x));
    int iy2 = static_cast<int>(std::round(pt2.y));

    // Bresenham
    int dx = std::abs(ix2 - ix1), sx = ix1 < ix2 ? 1 : -1;
    int dy = -std::abs(iy2 - iy1), sy = iy1 < iy2 ? 1 : -1;
    int err = dx + dy;

    int x = ix1;
    int y = iy1;
    while (true) {
        fillPixel(renderBuffer, x, y, color);
        if (x == ix2 && y == iy2) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y += sy;
        }
    }
}


void drawEllipse(const SVGNode* node,
                 std::vector<std::vector<glm::vec4>>& renderBuffer,
                 const glm::mat3& transform,
                 int rWidth,
                 int rHeight) {
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    float rx = getFloatAttribute(node, "rx");
    float ry = getFloatAttribute(node, "ry");

    glm::vec4 color = getFillColor(node);
    if (color.a == 0.0f) {
        return;
    }

    glm::vec3 newC = transform * glm::vec3(cx, cy, 1.0f);
    glm::vec3 radiusPoint = transform * glm::vec3(cx + rx, cy, 1.0f);
    float newRx = glm::distance(newC, radiusPoint);
    radiusPoint = transform * glm::vec3(cx, cy + ry, 1.0f);
    float newRy = glm::distance(newC, radiusPoint);
    float newCx = newC.x;
    float newCy = newC.y;

    int minX = static_cast<int>(std::floor(newCx - newRx));
    int maxX = static_cast<int>(std::ceil(newCx + newRx));
    int minY = static_cast<int>(std::floor(newCy - newRy));
    int maxY = static_cast<int>(std::ceil(newCy + newRy));

    for (int px = minX; px <= maxX; ++px) {
        for (int py = minY; py <= maxY; ++py) {

            float dx = (px - newCx) / newRx;
            float dy = (py - newCy) / newRy;
            if (dx*dx + dy*dy <= 1.0f) {

                glm::vec3 localPos(px, py, 1.0f);
                glm::vec3 worldPos = localPos;
                int finalX = static_cast<int>(std::round(worldPos.x));
                int finalY = static_cast<int>(std::round(worldPos.y));
                fillPixel(renderBuffer, finalX, finalY, color);
            }
        }
    }
}

void drawPath(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight) {
    // TODO:
    std::cout << "[drawPath] Not implemented.\n";
}

void drawPolygon(const SVGNode* node,
                 std::vector<std::vector<glm::vec4>>& renderBuffer,
                 const glm::mat3& transform,
                 int rWidth,
                 int rHeight) {

    std::vector<glm::vec2>  points;
    std::string pointsStr = node->getAttribute("points");

    std::replace(pointsStr.begin(), pointsStr.end(), ',', ' ');
    std::vector<float> nums;

    std::istringstream iss(pointsStr);
    for (std::string s; iss >> s;) {
        nums.push_back(std::stof(s));
    }

    for (int i = 0; i < nums.size(); i += 2) {
        glm::vec2 transformedPoint = transform * glm::vec3(nums[i], nums[i + 1], 1.0f);
        points.push_back(transformedPoint);
    }

    float minX,minY,maxX,maxY;
    minX = minY = std::numeric_limits<float>::max();
    maxX = maxY = std::numeric_limits<float>::min();
    for (auto& point : points) {
        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);
        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
    }
    for (auto po : points) {
        qInfo() << po.x << po.y;
    }
    for (int px = static_cast<int>(std::floor(minX)); px <= static_cast<int>(std::ceil(maxX)); ++px) {
        for (int py = static_cast<int>(std::floor(minY)); py <= static_cast<int>(std::ceil(maxY)); ++py) {
            if (isInPolygon(px, py, points)) {
                glm::vec3 localPos(px, py, 1.0f);
                glm::vec3 worldPos = localPos;
                int finalX = static_cast<int>(std::round(worldPos.x));
                int finalY = static_cast<int>(std::round(worldPos.y));
                fillPixel(renderBuffer, finalX, finalY, getFillColor(node));
            }
        }
    }

}

void drawPolyline(const SVGNode* node,
                  std::vector<std::vector<glm::vec4>>& renderBuffer,
                  const glm::mat3& transform,
                  int rWidth,
                  int rHeight) {

    std::vector<glm::vec2>  points;
    std::string pointsStr = node->getAttribute("points");

    std::replace(pointsStr.begin(), pointsStr.end(), ',', ' ');
    std::vector<float> nums;

    std::istringstream iss(pointsStr);
    for (std::string s; iss >> s;) {
        nums.push_back(std::stof(s));
    }

    for (int i = 0; i < nums.size(); i += 2) {
        glm::vec2 transformedPoint = transform * glm::vec3(nums[i], nums[i + 1], 1.0f);
        points.push_back(transformedPoint);
    }

    float minX,minY,maxX,maxY;
    minX = minY = std::numeric_limits<float>::max();
    maxX = maxY = std::numeric_limits<float>::min();
    for (auto& point : points) {
        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);
        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
    }

    glm::vec4 color = getStrokeColor(node);

    for (int i=0;i<points.size();i++) {
        // Use Bresenham's line algorithm to draw lines between points
        int x1 = static_cast<int>(points[i].x);
        int y1 = static_cast<int>(points[i].y);
        int x2 = static_cast<int>(points[(i+1)%points.size()].x);
        int y2 = static_cast<int>(points[(i+1)%points.size()].y);

        int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy;

        int x = x1;
        int y = y1;

        while (true) {
            fillPixel(renderBuffer, x, y, color);
            if (x == x2 && y == y2) break;
            int e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y += sy;
            }
        }
    }
}

void drawText(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight) {
    // Too Difficult for pixel-level rendering
}

static void renderNode(SVGNode* node,
                       std::vector<std::vector<glm::vec4>>& renderBuffer,
                       int rWidth,
                       int rHeight) {
    if (!node) return;

    SVGElements type = node->getType();

    glm::mat3 transform = node->getTransformMatrix();

    switch (type) {
        case SVGElements::Rect:
            drawRect(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::Circle:
            drawCircle(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::Line:
            drawLine(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::Ellipse:
            drawEllipse(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::Path:
            drawPath(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::Polygon:
            drawPolygon(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::Polyline:
            drawPolyline(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::Text:
            drawText(node, renderBuffer, transform, rWidth, rHeight);
            break;
        case SVGElements::G:
        case SVGElements::Svg:
        case SVGElements::Unknown:
        default:
            break;
    }

    for (SVGNode* child : node->getChildren()) {
        renderNode(child, renderBuffer, rWidth, rHeight);
    }
}

void SVGRenderer(std::vector<std::vector<glm::vec4>>& renderBuffer,
                 SVGNode* root,
                 int rWidth,
                 int rHeight) {
    if (!root) return;

    renderNode(root, renderBuffer, rWidth, rHeight);
}
