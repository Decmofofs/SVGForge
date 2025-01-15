#ifndef SVGNODE_H
#define SVGNODE_H
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include "SVGElements.h" // 包含 SVGElements 枚举定义

class SVGNode {
public:
    // 构造函数和析构函数
    SVGNode(SVGElements type = SVGElements::Unknown);
    ~SVGNode();

    // 设置和获取属性
    void setAttribute(const std::string& name, const std::string& value);
    std::string getAttribute(const std::string& name) const;

    // 添加和获取子节点
    void addChild(SVGNode* child);
    const std::vector<SVGNode*>& getChildren() const;

    // 获取节点类型
    SVGElements getType() const;

    void setTransformMatrix(const glm::mat3& matrix);

private:
    SVGElements type;                          // 节点类型
    std::map<std::string, std::string> attributes; // 属性集合
    std::vector<SVGNode*> children;           // 子节点集合

    glm::mat3 transformMatrix;
};

#endif // SVGNODE_H
