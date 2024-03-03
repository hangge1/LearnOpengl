
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
    
    Shader shader("res/shader/vertex.shader", "res/shader/fragment.shader");


    GLfloat vertices[] = {
         0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // ���Ͻ�
         0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // ���½�
        -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // ���½�
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f  // ���Ͻ�
    };

    GLuint indices[] = { // ע��������0��ʼ! 
        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };

    //��ʼ��VBO VAO 
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
        GLuint VBO;
        glGenBuffers(1, &VBO);
        // 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //3. EBO
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        //4. ���ö�������ָ��
            //(1)����index
            //(2)count
            //(3)���ݻ�������
            //(4)һ���������Ե��ܲ���
            //(5)��ǰ���Ե���ʼƫ��
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(1);
    //4. ���VAO
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window))
    {
        // ����¼�
        glfwPollEvents();

        // ���
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //��Ⱦ������
        shader.Bind();
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // ��������
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
    // �ر�Ӧ�ó���
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}