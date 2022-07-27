#include "wall.h"

Wall::Wall(float* vertices, int vertex_size, int vertex_stride, int vertices_size)
{
    this->vertices = vertices;
    this->indices = indices;
    this->vertex_size = vertex_size;
    this->vertex_stride = vertex_stride;
    this->vertices_size = vertices_size;
    this->indices_size = indices_size;
    this->get_vao();
}

void Wall::get_vao()
{
    unsigned int vao, vbo;
    // vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vertex_size, GL_FLOAT, GL_FALSE, vertex_stride*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_stride*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_stride*sizeof(float), (void*)(6*sizeof(float)));
    this->vao = vao;
}

glm::mat4 Wall::set_pos(glm::vec3 pos, Shader shader)
{
    this->position = pos;
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    shader.setMat4("model", model);
    glm::mat4 transpose_inverse = glm::transpose(glm::inverse(model));
    shader.setMat4("transpose_inverse", transpose_inverse);
    return model;
}

void Wall::render_wall(Shader shader)
{
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Wall::get_texture(unsigned int texture_id)
{
    this->texture_id = texture_id;
}


