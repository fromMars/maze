#include "light.h"

Light::Light(glm::vec3 position=glm::vec3(100.0, 100.0, 100.0), glm::vec3 color=glm::vec3(1.0, 1.0, 1.0))
{
    this->position = position;
    this->color = color;
}
