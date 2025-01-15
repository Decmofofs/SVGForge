//
// Created by Decmo on 25-1-16.
//
#include "HelperFunctions.h"

#include <algorithm>

glm::mat3 scale3x3(float scaleX, float scaleY) {
    glm::mat3 scaleMatrix(1.0f); // 单位矩阵

    // 设置缩放比例
    scaleMatrix[0][0] = scaleX; // x 缩放
    scaleMatrix[1][1] = scaleY; // y 缩放

    return scaleMatrix;
}

glm::mat3 getScale(const SVGNode* root) {
    // 从根节点提取属性
    std::string viewBoxStr = root->getAttribute("viewBox");
    std::string widthStr = root->getAttribute("width");
    std::string heightStr = root->getAttribute("height");

    // 默认 viewBox 为 0 0 300 150



    // 默认宽高为 300x150
    float width = 300.0f, height = 150.0f;

    float viewBoxMinX = 0.0f, viewBoxMinY = 0.0f;
    float viewBoxWidth = width, viewBoxHeight = height;
    // 解析 viewBox，如果存在
    if (!viewBoxStr.empty()) {
        std::istringstream stream(viewBoxStr);
        stream >> viewBoxMinX >> viewBoxMinY >> viewBoxWidth >> viewBoxHeight;
    }


    // 解析 width 和 height，如果存在
    if (!widthStr.empty()) width = std::stof(widthStr);
    if (!heightStr.empty()) height = std::stof(heightStr);

    // 计算缩放比例
    float scaleX = width / viewBoxWidth;
    float scaleY = height / viewBoxHeight;

    // 构造缩放矩阵
    return scale3x3(scaleX, scaleY);
}

std::vector<float> parseNumbers(const std::string& str)
{
    std::vector<float> result;
    std::string cleanStr = str;

    // 替换逗号为空格，方便用 stream 读数
    std::replace(cleanStr.begin(), cleanStr.end(), ',', ' ');

    std::stringstream ss(cleanStr);
    float val;
    while (ss >> val) {
        result.push_back(val);
    }
    return result;
}

glm::mat3 parseTransform(const std::string& transformStr)
{
    glm::mat3 result(1.0f); // 单位矩阵

    // 我们粗略地逐一查找: 找到 "xxx(" 到 匹配的 ")" 为一个命令
    size_t start = 0;
    while (true) {
        // 找 transform 名字和左括号
        size_t leftParen = transformStr.find('(', start);
        if (leftParen == std::string::npos) {
            break;
        }
        // 找对应右括号
        size_t rightParen = transformStr.find(')', leftParen);
        if (rightParen == std::string::npos) {
            break; // 未匹配到，退出
        }

        // 取出 "xxx" 作为命令名字 (例如 "translate", "rotate"...)
        std::string cmd = transformStr.substr(start, leftParen - start);
        // 去除空白
        auto trimPos = cmd.find_last_not_of(" \t\r\n");
        if (trimPos != std::string::npos) {
            cmd.erase(trimPos + 1);
        }

        // 取出括号内的内容
        std::string inside = transformStr.substr(leftParen + 1, rightParen - (leftParen + 1));

        // 将命令名转成小写，便于比较
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c){ return std::tolower(c); });

        // 解析里面的数字
        std::vector<float> nums = parseNumbers(inside);

        // 针对不同命令，计算局部矩阵 localMat
        glm::mat3 localMat(1.0f);

        if (cmd.find("translate") != std::string::npos) {
            // translate(tx, ty)，若只给了 tx 则 ty=0
            float tx = nums.size() > 0 ? nums[0] : 0.0f;
            float ty = nums.size() > 1 ? nums[1] : 0.0f;
            localMat[2][0] = tx; // 2D 仿射：平移可以放在第三列
            localMat[2][1] = ty;
        }
        else if (cmd.find("scale") != std::string::npos) {
            // scale(sx, sy)，若只给了 sx 则 sy=sx
            float sx = nums.size() > 0 ? nums[0] : 1.0f;
            float sy = nums.size() > 1 ? nums[1] : sx;
            localMat[0][0] = sx;
            localMat[1][1] = sy;
        }
        else if (cmd.find("rotate") != std::string::npos) {
            // rotate(angle) 或 rotate(angle, cx, cy)
            // 注意：SVG 中 rotate(angle, cx, cy) 意义是先 translate(cx, cy)，再 rotate(angle)，再 translate(-cx, -cy)
            float angle = nums.size() > 0 ? nums[0] : 0.0f; // 度数
            float cx = (nums.size() > 1 ? nums[1] : 0.0f);
            float cy = (nums.size() > 2 ? nums[2] : 0.0f);

            // 如果有中心点，则先平移到原点，再旋转，再平移回去
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
                // 合并
                localMat = t1 * r * t2;
            }
            else {
                // rotate(angle) 以 (0,0) 为中心
                float rad = angle * 3.1415926535f / 180.0f;
                float c = cos(rad);
                float s = sin(rad);
                localMat[0][0] =  c; localMat[0][1] = s;
                localMat[1][0] = -s; localMat[1][1] = c;
            }
        }
        else if (cmd.find("matrix") != std::string::npos) {
            // matrix(a,b,c,d,e,f) 对应:
            // [ a   c   e ]
            // [ b   d   f ]
            // [ 0   0   1 ]
            // 参考 SVG 2D 变换
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
        // 乘到 result 上（注意顺序：SVG 中 transform 列表是从左到右依次作用）
        result = result * localMat;

        // 更新 start，继续查找下一个 transform
        start = rightParen + 1;
    }

    return result;
}

void computeTransform(SVGNode* root)
{
    if (!root) return;

    // 利用一个 Lambda 进行深度或广度递归均可，这里以深度优先为例
    std::function<void(SVGNode*, const glm::mat3&)> traverse =
        [&](SVGNode* node, const glm::mat3& parentMatrix)
        {
            // 1. 解析当前节点的 transform 属性
            std::string tStr = node->getAttribute("transform"); // 可能为空
            glm::mat3 localMat = parseTransform(tStr);

            // 2. 计算当前节点的最终 transform
            glm::mat3 currentMat = parentMatrix * localMat;
            node->setTransformMatrix(currentMat);

            // 3. 递归处理子节点
            for (auto child : node->getChildren()) {
                traverse(child, currentMat);
            }
        };

    // 从根节点开始，父矩阵默认为单位阵
    traverse(root, glm::mat3(1.0f));
}
