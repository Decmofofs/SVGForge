//
// Created by Decmo on 25-1-15.
//

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QPixmap>
#include <glm/glm.hpp>
inline float scale = 1.0;
inline QPixmap *globalPix;
inline int renderFinished = 0;

inline glm::mat3 globalTransformMatrix = glm::mat3(1.0f);

#endif //GLOBALDATA_H
