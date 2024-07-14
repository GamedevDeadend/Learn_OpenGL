#include <GLAD/include/glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>
#include <iostream>
#include "Application.h"

/// <summary>
/// Function to Intialize and Configure GLFW
/// </summary>
void Intialize_And_Configure_GLFW()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

/// <summary>
/// Function to Create Window Using GLFW Lib
/// </summary>
/// <returns></returns>
GLFWwindow* CreateWindow()
{
    GLFWwindow* window = glfwCreateWindow(800, 800, "Graphics Engine", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to Create Window " << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    return window;
}

/// <summary>
/// Intialize GLAD Loader
/// </summary>
/// <returns></returns>
int Intialize_GLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to Intialize GLAD" << std::endl;
        return -1;
    }

    return 1;
}

/// <summary>
/// Function To Process Inputs
/// </summary>
/// <param name="window"></param>
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/// <summary>
/// OpenGL Callback function which will be invoked whenever window is resized 
/// </summary>
/// <param name="window"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/// <summary>
/// Process of Creating VBO to send vertices information to Graphic Memory (Vertex Shader)
/// </summary>
void SendingDataToVertexShader()
{
    //Vertex data
    float vertices[] = {
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        -0.0f,0.5f,0.0f,
    };

    //declaring Vertex BufferObject
    unsigned int vbo;

    //Generate Buffer
    glGenBuffers(1, &vbo);

    //Bind Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Allocate vertex data to bounded buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
}

int main()
{
    Intialize_And_Configure_GLFW();

    GLFWwindow* window = CreateWindow();
    if (window == nullptr)
    {
        return -1;
    }

    if (Intialize_GLAD() == -1)
    {
        return -1;
    }

    //OpenGL viewport Size
    glViewport(0, 0, 800, 800);

    SendingDataToVertexShader();

    //Set Framebuffer callback function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
