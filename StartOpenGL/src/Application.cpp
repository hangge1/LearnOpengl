
#include <iostream>
#include <string>

#include "shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLFWwindow* InitWindow(int height, int width,const std::string& title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(height, width, title.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return nullptr;
    }

    int framewidth, frameheight;
    glfwGetFramebufferSize(window, &framewidth, &frameheight);
    glViewport(0, 0, framewidth, frameheight);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

int main()
{
    GLFWwindow* window = InitWindow(800, 600, "LearnOpenGL");
    if (nullptr == window)
    {
        return -1;
    }

    GLfloat vertices[] = {
       -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 
    };

    GLuint indices[] = {
        0, 1, 2
    };


    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
        GLuint VBO;
        glGenBuffers(1, &VBO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //3. EBO
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        //4. 设置顶点属性指针
            //(1)属性index
            //(2)count
            //(3)数据基本类型
            //(4)一个顶点属性的总步长
            //(5)当前属性的起始偏移
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(1);
    //4. 解绑VAO
    glBindVertexArray(0);


    Shader shader("res/shader/default.vertex", "res/shader/default.fragment");


    while (!glfwWindowShouldClose(window))
    {
        // 检查事件
        glfwPollEvents();

        // 清空
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //渲染三角形
        shader.Bind();
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 交换缓冲
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}