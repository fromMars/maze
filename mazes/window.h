#ifndef WINDOW_H
#define WINDOW_H

#include "wall.h"

class Window
{
public:
    int width, height;
    char* title;
    GLFWwindow* window;
    Camera camera;
    //float delta_time;

    Window(int width, int height, char* title);
    GLFWwindow* init_window();
    //void input_process(GLFWwindow* Window);

private:
    /*float last_x;
    float last_y;
    bool first_mouse;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
    void scroll_callback(GLFWwindow* window, double x_off, double y_off);*/
};

#endif // WINDOW_H
