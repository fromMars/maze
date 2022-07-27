#ifndef LIGHT_H
#define LIGHT_H
#include "wall.h"

class Light
{
public:
    glm::vec3 position;
    glm::vec3 color;
    Light(glm::vec3 position, glm::vec3 color);
};

#endif // LIGHT_H
