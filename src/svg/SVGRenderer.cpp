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
// ================ 工具函数：渲染缓冲区访问 ================== //

/// 填充像素 (x, y)，要求 0 <= x < rWidth, 0 <= y < rHeight
inline void fillPixel(std::vector<std::vector<glm::vec4>>& renderBuffer,
                      int x, int y,
                      const glm::vec4& color)
{
    int rHeight = static_cast<int>(renderBuffer.size());
    if (rHeight == 0) return;
    int rWidth = static_cast<int>(renderBuffer[0].size());

    if (x >= 0 && x < rWidth && y >= 0 && y < rHeight) {
        renderBuffer[x][y] = color;
    }
}

/// 从节点中获取某个属性的浮点数值，若属性不存在则返回默认值
inline float getFloatAttribute(const SVGNode* node, const std::string& name, float defaultVal = 0.0f) {
    std::string valStr = node->getAttribute(name);
    if (valStr.empty()) {
        return defaultVal;
    }
    return std::stof(valStr);
}

/// 从节点中获取 "fill" 属性对应的颜色，若无则默认返回白色
inline glm::vec4 getFillColor(const SVGNode* node) {
    // 简化处理，只解析类似 "#RRGGBB" 或 "rgb(r, g, b)"，不含透明度
    std::string fillStr = node->getAttribute("fill");
    if (fillStr.empty() || fillStr == "none") {
        return glm::vec4(0.0f);  // 无填充
    }

    if (fillStr[0] == '#') {
        // 假设格式 "#RRGGBB"
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

    return glm::vec4({255,0,0,0.5});
}

// ================ 简单形状绘制示例 ================== //

// 1. 绘制 <rect>
void drawRect(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight)
{
    float x = getFloatAttribute(node, "x");
    float y = getFloatAttribute(node, "y");
    float w = getFloatAttribute(node, "width");
    float h = getFloatAttribute(node, "height");

    glm::vec4 color = getFillColor(node);
    qInfo() << x << y << w << h;
    if (color.a == 0.0f) {
        // alpha = 0 或者 fill="none"，不做任何填充
        return;
    }

    // 简单演示：逐像素填充矩形范围
    // （真实 SVG 要考虑 transform 后的四点再取 bounding box，这里只做最小示例）
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int x1 = static_cast<int>(std::ceil(x + w));
    int y1 = static_cast<int>(std::ceil(y + h));

    // 应用 transform，将像素坐标转换后再填充
    // 实际中要对矩形四个角做 transform，再取最小外包框，这里仅做非常简化处理
    qInfo() << x0 << y0 << x1 << y1;
    for (int px = x0; px < x1; ++px) {
        for (int py = y0; py < y1; ++py) {
            // 应用 transform
            glm::vec3 localPos(px, py, 1.0f);
            glm::vec3 worldPos = transform * localPos;

            // 取整后填充
            int finalX = static_cast<int>(std::round(worldPos.x));
            int finalY = static_cast<int>(std::round(worldPos.y));

            fillPixel(renderBuffer, finalX, finalY, color);
        }
    }
}

// 2. 绘制 <circle>
void drawCircle(const SVGNode* node,
                std::vector<std::vector<glm::vec4>>& renderBuffer,
                const glm::mat3& transform,
                int rWidth,
                int rHeight)
{
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    float r  = getFloatAttribute(node, "r");

    glm::vec4 color = getFillColor(node);
    if (color.a == 0.0f) {
        return;
    }

    // 取圆的外包矩形 [cx-r, cx+r] x [cy-r, cy+r]
    int minX = static_cast<int>(std::floor(cx - r));
    int maxX = static_cast<int>(std::ceil(cx + r));
    int minY = static_cast<int>(std::floor(cy - r));
    int maxY = static_cast<int>(std::ceil(cy + r));

    // 像素级判断是否落在圆内
    for (int px = minX; px <= maxX; ++px) {
        for (int py = minY; py <= maxY; ++py) {
            // 判断 (px,py) 到 (cx,cy) 的距离
            float dx = px - cx;
            float dy = py - cy;
            if (dx*dx + dy*dy <= r*r) {
                // 应用 transform
                glm::vec3 localPos(px, py, 1.0f);
                glm::vec3 worldPos = transform * localPos;
                int finalX = static_cast<int>(std::round(worldPos.x));
                int finalY = static_cast<int>(std::round(worldPos.y));
                fillPixel(renderBuffer, finalX, finalY, color);
            }
        }
    }
}

// 3. 绘制 <line>
void drawLine(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight)
{
    float x1 = getFloatAttribute(node, "x1");
    float y1 = getFloatAttribute(node, "y1");
    float x2 = getFloatAttribute(node, "x2");
    float y2 = getFloatAttribute(node, "y2");

    // 这里不解析 stroke 属性，仅示意用白色
    glm::vec4 color(1.0f);

    // 简化：Bresenham 或 DDA 画线
    // 1) 做 transform -> (worldX1, worldY1), (worldX2, worldY2)
    glm::vec3 pt1 = transform * glm::vec3(x1, y1, 1.0f);
    glm::vec3 pt2 = transform * glm::vec3(x2, y2, 1.0f);

    int ix1 = static_cast<int>(std::round(pt1.x));
    int iy1 = static_cast<int>(std::round(pt1.y));
    int ix2 = static_cast<int>(std::round(pt2.x));
    int iy2 = static_cast<int>(std::round(pt2.y));

    // 简易的 Bresenham 算法
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

// 4. 其他类型 (Ellipse / Polygon / Polyline / Path / Text / G / Svg / Unknown)
//    - 这里仅示意，留空或做简单打印，可自行扩展
void drawEllipse(const SVGNode* node,
                 std::vector<std::vector<glm::vec4>>& renderBuffer,
                 const glm::mat3& transform,
                 int rWidth,
                 int rHeight)
{
    // TODO: 与 circle 类似，但要区分 rx, ry
    std::cout << "[drawEllipse] Not implemented.\n";
}

void drawPath(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight)
{
    // TODO: 解析 d 属性 -> 拆分命令 -> 逐段绘制
    std::cout << "[drawPath] Not implemented.\n";
}

void drawPolygon(const SVGNode* node,
                 std::vector<std::vector<glm::vec4>>& renderBuffer,
                 const glm::mat3& transform,
                 int rWidth,
                 int rHeight)
{
    // TODO: 解析 points -> 扫描线填充
    std::cout << "[drawPolygon] Not implemented.\n";
}

void drawPolyline(const SVGNode* node,
                  std::vector<std::vector<glm::vec4>>& renderBuffer,
                  const glm::mat3& transform,
                  int rWidth,
                  int rHeight)
{
    // TODO: 类似 polygon，但不做填充
    std::cout << "[drawPolyline] Not implemented.\n";
}

void drawText(const SVGNode* node,
              std::vector<std::vector<glm::vec4>>& renderBuffer,
              const glm::mat3& transform,
              int rWidth,
              int rHeight)
{
    // TODO: 实现文字渲染（字体解析、排版等）
    std::cout << "[drawText] Not implemented.\n";
}

// ================ 核心渲染函数 ================== //

/// 递归渲染单个节点
static void renderNode(SVGNode* node,
                       std::vector<std::vector<glm::vec4>>& renderBuffer,
                       int rWidth,
                       int rHeight)
{
    if (!node) return;

    // 获取节点类型
    SVGElements type = node->getType();
    // 获取节点的最终变换矩阵
    glm::mat3 transform = node->getTransformMatrix();

    // 根据类型分发到具体绘制函数
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
            // G / Svg / Unknown：自己本身不直接绘制形状，但可能有子节点
            break;
    }

    // 递归渲染子节点
    for (SVGNode* child : node->getChildren()) {
        renderNode(child, renderBuffer, rWidth, rHeight);
    }
}

/// 对外暴露的渲染接口：渲染整个 root 树
void SVGRenderer(std::vector<std::vector<glm::vec4>>& renderBuffer,
                 SVGNode* root,
                 int rWidth,
                 int rHeight)
{
    if (!root) return;

    // 假设此时 root->transformMatrix 已经通过 computeTransform 等函数计算好
    // 或者是单位矩阵

    // 从 root 节点开始递归渲染
    renderNode(root, renderBuffer, rWidth, rHeight);
}
