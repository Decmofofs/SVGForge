
#include "HelperFunctions.h"

#include <algorithm>
#include <QDebug>
glm::mat3 scale3x3(float scaleX, float scaleY) {
    glm::mat3 scaleMatrix(1.0f); 

    scaleMatrix[0][0] = scaleX;
    scaleMatrix[1][1] = scaleY; 

    return scaleMatrix;
}

glm::mat3 getScale(const SVGNode* root) {
    std::string viewBoxStr = root->getAttribute("viewBox");
    std::string widthStr = root->getAttribute("width");
    std::string heightStr = root->getAttribute("height");

    float width = 300.0f, height = 150.0f;

    if (!widthStr.empty()) width = std::stof(widthStr);
    if (!heightStr.empty()) height = std::stof(heightStr);

    float viewBoxMinX = 0.0f, viewBoxMinY = 0.0f;
    float viewBoxWidth = width, viewBoxHeight = height;

    if (!viewBoxStr.empty()) {
        std::istringstream stream(viewBoxStr);
        stream >> viewBoxMinX >> viewBoxMinY >> viewBoxWidth >> viewBoxHeight;
    }


    float scaleX = width / viewBoxWidth;
    float scaleY = height / viewBoxHeight;

    return scale3x3(scaleX, scaleY);
}

std::vector<float> parseNumbers(const std::string& str) {
    std::vector<float> result;
    std::string cleanStr = str;

    // Replace all commas with spaces
    std::replace(cleanStr.begin(), cleanStr.end(), ',', ' ');

    std::stringstream ss(cleanStr);
    float val;
    while (ss >> val) {
        result.push_back(val);
    }
    return result;
}

glm::mat3 parseTransform(const std::string& transformStr) {
    glm::mat3 result(1.0f);

    size_t start = 0;
    while (true) {

        size_t leftParen = transformStr.find('(', start);
        if (leftParen == std::string::npos) {
            break;
        }

        size_t rightParen = transformStr.find(')', leftParen);
        if (rightParen == std::string::npos) {
            break;
        }

        std::string cmd = transformStr.substr(start, leftParen - start);

        auto trimPos = cmd.find_last_not_of(" \t\r\n");
        if (trimPos != std::string::npos) {
            cmd.erase(trimPos + 1);
        }

        std::string inside = transformStr.substr(leftParen + 1, rightParen - (leftParen + 1));

        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c){ return std::tolower(c); });

        std::vector<float> nums = parseNumbers(inside);

        glm::mat3 localMat(1.0f);

        if (cmd.find("translate") != std::string::npos) {

            float tx = nums.size() > 0 ? nums[0] : 0.0f;
            float ty = nums.size() > 1 ? nums[1] : 0.0f;
            localMat[2][0] = tx;
            localMat[2][1] = ty;
        }
        else if (cmd.find("scale") != std::string::npos) {

            float sx = nums.size() > 0 ? nums[0] : 1.0f;
            float sy = nums.size() > 1 ? nums[1] : sx;
            localMat[0][0] = sx;
            localMat[1][1] = sy;
        }
        else if (cmd.find("rotate") != std::string::npos) {

            float angle = nums.size() > 0 ? nums[0] : 0.0f;
            float cx = (nums.size() > 1 ? nums[1] : 0.0f);
            float cy = (nums.size() > 2 ? nums[2] : 0.0f);

            if (nums.size() > 1) {
                // T(cx,cy)
                glm::mat3 t1(1.0f);
                t1[2][0] = cx;
                t1[2][1] = cy;
                // R(angle)
                float rad = angle * 3.1415926535f / 180.0f;
                glm::mat3 r(1.0f);
                float c = cos(rad);
                float s = sin(rad);
                r[0][0] =  c; r[0][1] = s;
                r[1][0] = -s; r[1][1] = c;
                // T(-cx, -cy)
                glm::mat3 t2(1.0f);
                t2[2][0] = -cx;
                t2[2][1] = -cy;

                localMat = t1 * r * t2;
            }
            else {

                float rad = angle * 3.1415926535f / 180.0f;
                float c = cos(rad);
                float s = sin(rad);
                localMat[0][0] =  c; localMat[0][1] = s;
                localMat[1][0] = -s; localMat[1][1] = c;
            }
        }
        else if (cmd.find("matrix") != std::string::npos) {
            // matrix(a,b,c,d,e,f)
            // [ a   c   e ]
            // [ b   d   f ]
            // [ 0   0   1 ]

            if (nums.size() == 6) {
                float a = nums[0], b = nums[1], c = nums[2],
                      d = nums[3], e = nums[4], f = nums[5];
                localMat[0][0] = a;
                localMat[0][1] = c;
                localMat[1][0] = b;
                localMat[1][1] = d;
                localMat[2][0] = e;
                localMat[2][1] = f;
            }
        }
        else if (cmd.find("skewx") != std::string::npos) {
            // skewX(angle)
            qDebug() << "faeofbaoebfoafboubfoueafbieabfoaebfaeobu";
            float angle = nums.size() > 0 ? nums[0] : 0.0f;
            float rad = angle * 3.1415926535f / 180.0f;
            float t = tan(rad);
            localMat[1][0] = t;
        }
        else if (cmd.find("skewy") != std::string::npos) {
            // skewY(angle)
            float angle = nums.size() > 0 ? nums[0] : 0.0f;
            float rad = angle * 3.1415926535f / 180.0f;
            float t = tan(rad);
            localMat[0][1] = t;
        }

        result = result * localMat;

        start = rightParen + 1;
    }

    return result;
}

void computeTransform(SVGNode* root) {
    if (!root) return;

    std::function<void(SVGNode*, const glm::mat3&)> traverse =
        [&](SVGNode* node, const glm::mat3& parentMatrix) {

            std::string tStr = node->getAttribute("transform");
            glm::mat3 localMat = parseTransform(tStr);

            glm::mat3 currentMat = parentMatrix * localMat;
            node->setTransformMatrix(currentMat);

            for (auto child : node->getChildren()) {
                traverse(child, currentMat);
            }
        };

    traverse(root, glm::mat3(1.0f));
}

std::pair<float, float> getWidthAndHeight(const SVGNode* root) {
    if (!root) {
        throw std::invalid_argument("Root node is null.");
    }

    std::string widthStr = root->getAttribute("width");
    std::string heightStr = root->getAttribute("height");

    float width = widthStr.empty() ? 0.0f : std::stof(widthStr);
    float height = heightStr.empty() ? 0.0f : std::stof(heightStr);

    return {width, height};
}

void applyGlobalTransform(SVGNode* node, const glm::mat3& globalTransformMatrix) {
    if (!node) return;

    glm::mat3 currentMatrix = node->getTransformMatrix();

    glm::mat3 updatedMatrix = globalTransformMatrix * currentMatrix;
    node->setTransformMatrix(updatedMatrix);

    for (SVGNode* child : node->getChildren()) {
        applyGlobalTransform(child, globalTransformMatrix);
    }
}

void vectorToPixmap(std::vector<std::vector<glm::vec4> > &renderBuffer, QPixmap &pixmap) {
    QImage image(renderBuffer[0].size(), renderBuffer.size(), QImage::Format_ARGB32);

    for (size_t x = 0; x < renderBuffer.size(); x++) {
        for (size_t y = 0; y < renderBuffer[0].size(); y++) {
            glm::vec4 color = renderBuffer[x][y];

            image.setPixelColor(x, y, QColor::fromRgbF(color.r, color.g, color.b, color.a));
        }
    }

    pixmap = QPixmap::fromImage(image);
}

int isInPolygon(float x, float y, const std::vector<glm::vec2>& poly) {
    int init = 0;
    int flag = 0;

    for (int i=0;i<poly.size();i++) {
        glm::vec2 V1 = poly[(i+1)%poly.size()] - poly[i];
        glm::vec2 V2 = glm::vec2(x, y) - poly[i];
        float cross = V1.x * V2.y - V1.y * V2.x;
        if (init == 0) {
            if (cross > 0) flag = 1;
            else flag = -1;
            init = 1;
        }
        else {
            int fcross = cross > 0 ? 1 : -1;
            if (fcross != flag) return 0;
        }
    }
    return 1;
}

