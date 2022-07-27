#ifndef WALL_H
#define WALL_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "shader.h"

class Wall
{
public:
    glm::vec3 position;
    float* vertices;
    unsigned int* indices;
    unsigned int vao;
    int vertex_size, vertex_stride, vertices_size, indices_size;
    Wall(float* vertices, int vertex_size, int vertex_stride, int v_size);
    glm::mat4 set_pos(glm::vec3 pos, Shader shader);
    void get_vao();
    void render_wall(Shader shader);
    void get_texture(unsigned int texture_id);

private:
    unsigned int texture_id;
};

#endif // WALL_H
