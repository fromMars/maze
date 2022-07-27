#include "Maze.h"
#include "window.h"
#include "stb_image.h"
#include "light.h"
#include "shadow.h"
using namespace std;

// callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_off, double y_off);
void input_process(GLFWwindow* window);
void draw_maze_walls(std::vector< std::vector< std::vector< bool > > > maze, Wall wall, Shader shader);
unsigned int load_texture(string filepath);
bool collision_detection(Camera camera, Wall wall_block);
void render_scene(Shader shader, Maze maze, Wall wall, Wall floor);

// main window size
int width = 800;
int height =600;
char* title = "Maze";

// camera
Camera camera(glm::vec3(0.0, 0.0, 3.0));
float last_frame_time = 0;
float delta_time = 0;
float last_x = (float)width/2;
float last_y = (float)height/2;
bool first_mouse = true;

// maze size
int mz_width = 15;
int mz_height = 9;

// last position befor collision
glm::vec3 last_pos;
glm::vec3 init_pos;

int main()
{
    // maze map
    Maze maze(mz_width, mz_height);
    maze.createMaze();
    cout << "start pos:\t(" << maze.start_pos[0] << ", " << maze.start_pos[1] << ")" << endl;
    cout << "finish pos:\t(" << maze.finish_pos[0] << ", " << maze.finish_pos[1] << ")" << endl;
    glm::vec3 start_pos = glm::vec3(maze.start_pos[0], 0.0, maze.start_pos[1]);

    // start position
    glm::vec3 start_pos_yaw = glm::vec3(start_pos.x,start_pos.y,start_pos.z);
    init_pos = start_pos;
    if (start_pos.z == 0)
        start_pos_yaw.z = -start_pos_yaw.z - 2;
    else if (start_pos.x==0 && start_pos.z>0 && start_pos.z<mz_height)
        start_pos_yaw.x -= 2;
    else if (start_pos.x==mz_width-1 && start_pos.z>0 && start_pos.z<mz_height)
        start_pos_yaw.x +=2;
    else
        start_pos_yaw.z = start_pos_yaw.z + 2;
    //camera = Camera(start_pos_yaw);
    camera.Position = start_pos_yaw;
    camera.Front = glm::normalize(start_pos-start_pos_yaw);
    glm::mat4 view0 = camera.GetViewMatrix();
    std::cout<<start_pos_yaw.x<<", "<<start_pos_yaw.z<<std::endl;
    std::cout<<camera.Front.x<<", "<<camera.Front.z<<std::endl;

    // main window settings
    Window wnd(width, height, title);
    GLFWwindow* window = wnd.init_window();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // shaders
    Shader shader("shaders/wall.vs", "shaders/wall.fs");
    Shader depth_shader("shaders/depth_map.vs", "shaders/depth_map.fs");
    Shader debug_shader("shaders/debug_depth_map.vs", "shaders/debug_depth_map.fs");

    // lights
    Light light(glm::vec3(30.0f, 30.0f, 15.0f), glm::vec3(1.0f));

    // vertices indices
    /*
    float verteics[] {
        0.5, 0.5, 0.5, 1.0, 1.0,
        0.5, -0.5, 0.5, 1.0, 0.0,
        -0.5, -0.5, 0.5, 0.0, 0.0,
        -0.5, 0.5, 0.5, 0.0, 1.0,
        0.5, 0.5, -0.5, 0.0, 0.0,
        0.5, -0.5, -0.5, 0.0, 1.0,
        -0.5, -0.5, -0.5, 1.0, 1.0,
        -0.5, 0.5, -0.5, 1.0, 0.0
    };
    unsigned int indices[] {
        7, 3, 4, 0, 1, 4, 5, 7, 6, 3, 2, 0, 1 //, 5, 2, 6
    };
    float floor_verteics[] {
        0.5, 0.5, 0.5, 0.0, 0.0,
        -0.5, 0.5, 0.5, 10.0, 0.0,
        -0.5, 0.5, -0.5, 10.0, 10.0,
        0.5, 0.5, -0.5, 0.0, 10.0
    };
    unsigned int floor_indices[] {
        3, 0, 2, 1
    };
    */
    float cube_vertices[] = {
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -0.5f,  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         0.5f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
        // bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         0.5f,  0.5f , 0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
    };
    // clock-wise vertex
    float skybox_vertices[] = {
        // positions
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f
    };

    float floor_vertices[] = {
        0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 10.0, 10.0,
        0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 10.0, 0.0,
        -0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0,
        -0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0,
        -0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 0.0, 10.0,
        0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 10.0, 10.0
    };

    float quad_vertices[] = {
        -1.0, -1.0, 0.0, 0.0,
        1.0, -1.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        -1.0, 1.0, 0.0, 1.0,
        -1.0, -1.0, 0.0, 0.0
    };

    // wall
    Wall wall(cube_vertices, 3, 8, sizeof(cube_vertices));
    wall.get_texture(load_texture("resources/textures/wall.jpg"));
    // floor
    Wall floor(floor_vertices, 3, 8, sizeof(floor_vertices));
    floor.get_texture(load_texture("resources/textures/floor.jpg"));
    // shadows
    Shadow shadow(quad_vertices, sizeof(quad_vertices), light.position);
    //Shadow shadow(quad_vertices, sizeof(quad_vertices), light.position);

    // test object
    /*unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verteics), verteics, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    /*float last_frame_time = 0;
    float delta_time = 0;
    bool first_mouse = true;
    // attach camer to window
    wnd.camera = camera;*/

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /** when playing */
    // switch line mode, toggle birdview, hint arrow
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("depth_map", 1);

    shadow.get_depth_map();
    shadow.get_quad_buffer();
    debug_shader.use();
    debug_shader.setInt("depthMap", 0);

    while(!glfwWindowShouldClose(window))
    {
        // movement delta_time
        float current_frame_time = static_cast<float>(glfwGetTime());
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;
        // clear buffers
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shadow.gen_depth_framebuffer(depth_shader);
        depth_shader.use();
        //depth_shader.setVec3("light_pos", light.position);
        //depth_shader.setVec3("light_color", light.color);
        //depth_shader.setVec3("view_pos", camera.Position);
        glCullFace(GL_FRONT);
        render_scene(depth_shader, maze, wall, floor);
        glCullFace(GL_BACK);

        // end depth map framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width/(float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1.0f);
        // set light
        //glActiveTexture(GL_TEXTURE0);
        shader.use();
        shader.setVec3("light_pos", light.position);
        shader.setVec3("light_color", light.color);
        shader.setVec3("view_pos", camera.Position);
        // render scene
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setMat4("lightSpaceMatrix", shadow.gen_light_space_matrix());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow.depth_texture);
        render_scene(shader, maze, wall, floor);

        // debug
        /*debug_shader.use();
        glBindVertexArray(shadow.quad_vao);
        debug_shader.setFloat("near_plane", shadow.near_plane);
        debug_shader.setFloat("far_plane", shadow.far_plane);
        glBindTexture(GL_TEXTURE_2D, shadow.depth_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);*/

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
        // global input handler
        input_process(window);
    }

    //glDeleteVertexArrays(1, &vao);
    //glDeleteBuffers(1, &vbo);
    //glDeleteBuffers(1, &ebo);
    glfwTerminate();
    return 0;
}

void render_scene(Shader shader, Maze maze, Wall wall, Wall floor)
{
    // draw walls
    draw_maze_walls(maze.maze, wall, shader);
    // draw floor
    glm::mat4 model(1.0f);
    model = floor.set_pos(glm::vec3((float)mz_width/2.0, -1.01, (float)mz_height/2.0f), shader);
    shader.setMat4("model", glm::scale(model, glm::vec3((float)mz_width+5, 1.0f, (float)mz_height+5)));
    glm::mat4 transpose_inverse = glm::transpose(glm::inverse(model));
    shader.setMat4("transpose_inverse", transpose_inverse);
    floor.render_wall(shader);
}

// assume player size is a 0.2^3 cube,
// no collision:
//      distance(wall_block, player) > 0.5+0.2 = 0.7
bool collision_detection(Camera camera, Wall wall_block)
{
    float distance_player_wall = 0;
    glm::vec3 player_position = camera.Position;
    glm::vec3 wall_block_position = wall_block.position;
    distance_player_wall = glm::distance(player_position, wall_block_position);
    if (distance_player_wall<0.7)
    {
        //delta_time = 0;
        /*camera.Position = init_pos;
        camera = Camera(init_pos);
        camera.GetViewMatrix();
        std::cout<<"distance: "<<distance_player_wall<<"\t"<<last_pos.x<<", "<<last_pos.y<<", "<<last_pos.z<<std::endl;*/
        return true;

    }

    return false;
}

unsigned int load_texture(string filepath)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    int w, h, nr;
    unsigned char* data = stbi_load(filepath.c_str(), &w, &h, &nr, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
        std::cout<<"Failed to load texture: "<<filepath<<endl;
    stbi_image_free(data);
    return texture_id;
}

void draw_maze_walls(std::vector< std::vector< std::vector< bool > > > maze, Wall wall, Shader shader)
{
    for(unsigned int a = 0; a < maze.size(); a++){
        for(unsigned int b = 0; b < maze[a].size(); b++){
            if(maze[a][b][0]){
                wall.set_pos(glm::vec3(1.0*b, 0.0, 1.0*a), shader);
                wall.render_wall(shader);
                if(!collision_detection(camera, wall))
                {
                    //camera.Position = init_pos;
                    last_pos = camera.Position;
                }
                else
                {
                    //std::cout<<"last_pos:\t"<<last_pos.x<<","<<last_pos.z<<std::endl;
                    //camera.Position = init_pos;
                    float x = glm::roundEven(last_pos.x);
                    float z = glm::roundEven(last_pos.z);
                    //std::cout<<"roundEven:\t"<<x<<","<<z<<std::endl;
                    //std::cout<<"roundEven:\t"<<x<<","<<z<<std::endl;
                    camera.Position = glm::vec3(x, last_pos.y, z);
                    //std::cout<<"cameraPosition:\t"<<camera.Position.x<<","<<camera.Position.z<<std::endl;
                }
            }
        }
        wall.set_pos(glm::vec3(0.0, 0.0, 1.0*a), shader);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    float x = static_cast<float>(x_pos);
    float y = static_cast<float>(y_pos);
    if(first_mouse)
    {
        last_x = x;
        last_y = y;
        first_mouse = false;
    }
    float x_off = x - last_x;
    float y_off = y - last_y;
    last_x = x;
    last_y = y;
    camera.ProcessMouseMovement(x_off, -y_off);
}

void scroll_callback(GLFWwindow* window, double x_off, double y_off)
{
    camera.ProcessMouseScroll(static_cast<float>(y_off));
}

void input_process(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
    // stay on xz plane
    camera.Position.y = 0;
}
