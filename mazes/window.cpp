#include "window.h"

Window::Window(int width, int height, char* title)
{
    this->width = width;
    this->height = height;
    this->title = title;
    /*this->last_x = (float)width/2.0f;
    this->last_y = (float)height/2.0f;
    this->delta_time = 0;
    this->first_mouse = true;*/
}

GLFWwindow* Window::init_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    GLFWwindow(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
#endif // __APPLE__
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window==NULL)
    {
        std::cout<<"Failed to create window!"<<std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    /*glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)&framebuffer_size_callback);
    glfwSetCursorPosCallback(window, (GLFWcursorposfun)&mouse_callback);
    glfwSetScrollCallback(window, (GLFWscrollfun)&scroll_callback);*/
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"Failed to initialize GLAD!"<<std::endl;
        return NULL;
    }
    return window;
}

/*
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
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

void Window::scroll_callback(GLFWwindow* window, double x_off, double y_off)
{
    camera.ProcessMouseScroll(static_cast<float>(y_off));
}

void Window::input_process(GLFWwindow* window)
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
}*/
