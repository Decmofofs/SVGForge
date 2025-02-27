
#ifndef PREDEFINEDCOLORS_H
#define PREDEFINEDCOLORS_H

#include <map>
#include <string>
#include <glm/glm.hpp>

// 147 predefined SVG colors
std::map<std::string, glm::vec4> svgColorTable = {
    {"aliceblue", glm::vec4(240, 248, 255, 255) / 255.0f},
    {"antiquewhite", glm::vec4(250, 235, 215, 255) / 255.0f},
    {"aqua", glm::vec4(0, 255, 255, 255) / 255.0f},
    {"aquamarine", glm::vec4(127, 255, 212, 255) / 255.0f},
    {"azure", glm::vec4(240, 255, 255, 255) / 255.0f},
    {"beige", glm::vec4(245, 245, 220, 255) / 255.0f},
    {"bisque", glm::vec4(255, 228, 196, 255) / 255.0f},
    {"black", glm::vec4(0, 0, 0, 255) / 255.0f},
    {"blanchedalmond", glm::vec4(255, 235, 205, 255) / 255.0f},
    {"blue", glm::vec4(0, 0, 255, 255) / 255.0f},
    {"blueviolet", glm::vec4(138, 43, 226, 255) / 255.0f},
    {"brown", glm::vec4(165, 42, 42, 255) / 255.0f},
    {"burlywood", glm::vec4(222, 184, 135, 255) / 255.0f},
    {"cadetblue", glm::vec4(95, 158, 160, 255) / 255.0f},
    {"chartreuse", glm::vec4(127, 255, 0, 255) / 255.0f},
    {"chocolate", glm::vec4(210, 105, 30, 255) / 255.0f},
    {"coral", glm::vec4(255, 127, 80, 255) / 255.0f},
    {"cornflowerblue", glm::vec4(100, 149, 237, 255) / 255.0f},
    {"cornsilk", glm::vec4(255, 248, 220, 255) / 255.0f},
    {"crimson", glm::vec4(220, 20, 60, 255) / 255.0f},
    {"cyan", glm::vec4(0, 255, 255, 255) / 255.0f},
    {"darkblue", glm::vec4(0, 0, 139, 255) / 255.0f},
    {"darkcyan", glm::vec4(0, 139, 139, 255) / 255.0f},
    {"darkgoldenrod", glm::vec4(184, 134, 11, 255) / 255.0f},
    {"darkgray", glm::vec4(169, 169, 169, 255) / 255.0f},
    {"darkgreen", glm::vec4(0, 100, 0, 255) / 255.0f},
    {"darkkhaki", glm::vec4(189, 183, 107, 255) / 255.0f},
    {"darkmagenta", glm::vec4(139, 0, 139, 255) / 255.0f},
    {"darkolivegreen", glm::vec4(85, 107, 47, 255) / 255.0f},
    {"darkorange", glm::vec4(255, 140, 0, 255) / 255.0f},
    {"darkorchid", glm::vec4(153, 50, 204, 255) / 255.0f},
    {"darkred", glm::vec4(139, 0, 0, 255) / 255.0f},
    {"darksalmon", glm::vec4(233, 150, 122, 255) / 255.0f},
    {"darkseagreen", glm::vec4(143, 188, 143, 255) / 255.0f},
    {"darkslateblue", glm::vec4(72, 61, 139, 255) / 255.0f},
    {"darkslategray", glm::vec4(47, 79, 79, 255) / 255.0f},
    {"darkturquoise", glm::vec4(0, 206, 209, 255) / 255.0f},
    {"darkviolet", glm::vec4(148, 0, 211, 255) / 255.0f},
    {"deeppink", glm::vec4(255, 20, 147, 255) / 255.0f},
    {"deepskyblue", glm::vec4(0, 191, 255, 255) / 255.0f},
    {"dimgray", glm::vec4(105, 105, 105, 255) / 255.0f},
    {"dodgerblue", glm::vec4(30, 144, 255, 255) / 255.0f},
    {"firebrick", glm::vec4(178, 34, 34, 255) / 255.0f},
    {"floralwhite", glm::vec4(255, 250, 240, 255) / 255.0f},
    {"forestgreen", glm::vec4(34, 139, 34, 255) / 255.0f},
    {"fuchsia", glm::vec4(255, 0, 255, 255) / 255.0f},
    {"gainsboro", glm::vec4(220, 220, 220, 255) / 255.0f},
    {"ghostwhite", glm::vec4(248, 248, 255, 255) / 255.0f},
    {"gold", glm::vec4(255, 215, 0, 255) / 255.0f},
    {"goldenrod", glm::vec4(218, 165, 32, 255) / 255.0f},
    {"gray", glm::vec4(128, 128, 128, 255) / 255.0f},
    {"green", glm::vec4(0, 128, 0, 255) / 255.0f},
    {"greenyellow", glm::vec4(173, 255, 47, 255) / 255.0f},
    {"honeydew", glm::vec4(240, 255, 240, 255) / 255.0f},
    {"hotpink", glm::vec4(255, 105, 180, 255) / 255.0f},
    {"indianred", glm::vec4(205, 92, 92, 255) / 255.0f},
    {"indigo", glm::vec4(75, 0, 130, 255) / 255.0f},
    {"ivory", glm::vec4(255, 255, 240, 255) / 255.0f},
    {"khaki", glm::vec4(240, 230, 140, 255) / 255.0f},
    {"lavender", glm::vec4(230, 230, 250, 255) / 255.0f},
    {"lavenderblush", glm::vec4(255, 240, 245, 255) / 255.0f},
    {"lawngreen", glm::vec4(124, 252, 0, 255) / 255.0f},
    {"lemonchiffon", glm::vec4(255, 250, 205, 255) / 255.0f},
    {"lightblue", glm::vec4(173, 216, 230, 255) / 255.0f},
    {"lightcoral", glm::vec4(240, 128, 128, 255) / 255.0f},
    {"lightcyan", glm::vec4(224, 255, 255, 255) / 255.0f},
    {"lightgoldenrodyellow", glm::vec4(250, 250, 210, 255) / 255.0f},
    {"lightgreen", glm::vec4(144, 238, 144, 255) / 255.0f},
    {"lightgray", glm::vec4(211, 211, 211, 255) / 255.0f},
    {"lightpink", glm::vec4(255, 182, 193, 255) / 255.0f},
    {"lightsalmon", glm::vec4(255, 160, 122, 255) / 255.0f},
    {"lightseagreen", glm::vec4(32, 178, 170, 255) / 255.0f},
    {"lightskyblue", glm::vec4(135, 206, 250, 255) / 255.0f},
    {"lightslategray", glm::vec4(119, 136, 153, 255) / 255.0f},
    {"lightsteelblue", glm::vec4(176, 196, 222, 255) / 255.0f},
    {"lightyellow", glm::vec4(255, 255, 224, 255) / 255.0f},
    {"lime", glm::vec4(0, 255, 0, 255) / 255.0f},
    {"limegreen", glm::vec4(50, 205, 50, 255) / 255.0f},
    {"linen", glm::vec4(250, 240, 230, 255) / 255.0f},
    {"magenta", glm::vec4(255, 0, 255, 255) / 255.0f},
    {"maroon", glm::vec4(128, 0, 0, 255) / 255.0f},
    {"mediumaquamarine", glm::vec4(102, 205, 170, 255) / 255.0f},
    {"mediumblue", glm::vec4(0, 0, 205, 255) / 255.0f},
    {"mediumorchid", glm::vec4(186, 85, 211, 255) / 255.0f},
    {"mediumpurple", glm::vec4(147, 112, 219, 255) / 255.0f},
    {"mediumseagreen", glm::vec4(60, 179, 113, 255) / 255.0f},
    {"mediumslateblue", glm::vec4(123, 104, 238, 255) / 255.0f},
    {"mediumspringgreen", glm::vec4(0, 250, 154, 255) / 255.0f},
    {"mediumturquoise", glm::vec4(72, 209, 204, 255) / 255.0f},
    {"mediumvioletred", glm::vec4(199, 21, 133, 255) / 255.0f},
    {"midnightblue", glm::vec4(25, 25, 112, 255) / 255.0f},
    {"mintcream", glm::vec4(245, 255, 250, 255) / 255.0f},
    {"mistyrose", glm::vec4(255, 228, 225, 255) / 255.0f},
    {"moccasin", glm::vec4(255, 228, 181, 255) / 255.0f},
    {"navajowhite", glm::vec4(255, 222, 173, 255) / 255.0f},
    {"navy", glm::vec4(0, 0, 128, 255) / 255.0f},
    {"oldlace", glm::vec4(253, 245, 230, 255) / 255.0f},
    {"olive", glm::vec4(128, 128, 0, 255) / 255.0f},
    {"olivedrab", glm::vec4(107, 142, 35, 255) / 255.0f},
    {"orange", glm::vec4(255, 165, 0, 255) / 255.0f},
    {"orangered", glm::vec4(255, 69, 0, 255) / 255.0f},
    {"orchid", glm::vec4(218, 112, 214, 255) / 255.0f},
    {"palegoldenrod", glm::vec4(238, 232, 170, 255) / 255.0f},
    {"palegreen", glm::vec4(152, 251, 152, 255) / 255.0f},
    {"paleturquoise", glm::vec4(175, 238, 238, 255) / 255.0f},
    {"palevioletred", glm::vec4(219, 112, 147, 255) / 255.0f},
    {"papayawhip", glm::vec4(255, 239, 213, 255) / 255.0f},
    {"peachpuff", glm::vec4(255, 218, 185, 255) / 255.0f},
    {"peru", glm::vec4(205, 133, 63, 255) / 255.0f},
    {"pink", glm::vec4(255, 192, 203, 255) / 255.0f},
    {"plum", glm::vec4(221, 160, 221, 255) / 255.0f},
    {"powderblue", glm::vec4(176, 224, 230, 255) / 255.0f},
    {"purple", glm::vec4(128, 0, 128, 255) / 255.0f},
    {"red", glm::vec4(255, 0, 0, 255) / 255.0f},
    {"rosybrown", glm::vec4(188, 143, 143, 255) / 255.0f},
    {"royalblue", glm::vec4(65, 105, 225, 255) / 255.0f},
    {"saddlebrown", glm::vec4(139, 69, 19, 255) / 255.0f},
    {"salmon", glm::vec4(250, 128, 114, 255) / 255.0f},
    {"sandybrown", glm::vec4(244, 164, 96, 255) / 255.0f},
    {"seagreen", glm::vec4(46, 139, 87, 255) / 255.0f},
    {"seashell", glm::vec4(255, 245, 238, 255) / 255.0f},
    {"sienna", glm::vec4(160, 82, 45, 255) / 255.0f},
    {"silver", glm::vec4(192, 192, 192, 255) / 255.0f},
    {"skyblue", glm::vec4(135, 206, 235, 255) / 255.0f},
    {"slateblue", glm::vec4(106, 90, 205, 255) / 255.0f},
    {"slategray", glm::vec4(112, 128, 144, 255) / 255.0f},
    {"snow", glm::vec4(255, 250, 250, 255) / 255.0f},
    {"springgreen", glm::vec4(0, 255, 127, 255) / 255.0f},
    {"steelblue", glm::vec4(70, 130, 180, 255) / 255.0f},
    {"tan", glm::vec4(210, 180, 140, 255) / 255.0f},
    {"teal", glm::vec4(0, 128, 128, 255) / 255.0f},
    {"thistle", glm::vec4(216, 191, 216, 255) / 255.0f},
    {"tomato", glm::vec4(255, 99, 71, 255) / 255.0f},
    {"turquoise", glm::vec4(64, 224, 208, 255) / 255.0f},
    {"violet", glm::vec4(238, 130, 238, 255) / 255.0f},
    {"wheat", glm::vec4(245, 222, 179, 255) / 255.0f},
    {"white", glm::vec4(255, 255, 255, 255) / 255.0f},
    {"whitesmoke", glm::vec4(245, 245, 245, 255) / 255.0f},
    {"yellow", glm::vec4(255, 255, 0, 255) / 255.0f},
    {"yellowgreen", glm::vec4(154, 205, 50, 255) / 255.0f}
};


#endif //PREDEFINEDCOLORS_H
