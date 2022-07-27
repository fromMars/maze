#ifndef SHADOW_H
#define SHADOW_H
#include "wall.h"

class Shadow
{
public:
    unsigned int depth_map_fbo, depth_texture;
    unsigned int quad_vao, quad_vbo;
    float near_plane = 1.0f, far_plane = 100.0f;
    float* quad_vertices;
    int quad_vertices_size;
    glm::vec3 light_pos;
    Shadow(float* quad_vertices, int quad_vertices_size, glm::vec3 light_pos);
    void get_depth_map();
    void get_quad_buffer();
    void set_viewport();
    void gen_depth_framebuffer(Shader shader);
    glm::mat4 gen_light_space_matrix();

private:
    const unsigned int shadow_width = 1024, shadow_height = 1024;

};

#endif // SHADOW_H
