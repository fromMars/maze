#include "shadow.h"

Shadow::Shadow(float* quad_vertices, int quad_vertices_size, glm::vec3 light_pos)
{
    this->quad_vertices = quad_vertices;
    this->quad_vertices_size = quad_vertices_size;
    this->light_pos = light_pos;
}

void Shadow::get_depth_map()
{
    // depth framebuffer
    glGenFramebuffers(1, &depth_map_fbo);
    // depth texture
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
    //debug_depth_shader.use();
    //debug_depth_shader.setInt("depthMap", 0);

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}

void Shadow::get_quad_buffer()
{
    // quad buffer
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, quad_vertices_size, quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
}

void Shadow::set_viewport()
{
    glViewport(0, 0, shadow_width, shadow_height);
}

void Shadow::gen_depth_framebuffer(Shader shader)
{
    // depth map
    set_viewport();
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    // configure shader and matrix
    glm::mat4 lightSpaceMatrix = gen_light_space_matrix();
    shader.use();
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

glm::mat4 Shadow::gen_light_space_matrix()
{
    glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 light_view = glm::lookAt(light_pos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    return light_projection * light_view;
}

