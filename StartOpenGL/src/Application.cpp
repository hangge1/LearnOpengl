
#include <iostream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>

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

GLuint CompileShader(const std::string& shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const char* src = shaderSource.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

GLuint CreateShaderProgram(GLuint vs, GLuint fs)
{
    //������ɫ��
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::LINK_SHADER_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    //ʹ����ɫ��
    glUseProgram(shaderProgram);

    //������Դ
    glDeleteShader(vs);
    glDeleteShader(fs);
    return shaderProgram;
}

int main()
{
    GLFWwindow* window = InitWindow(800, 600, "LearnOpenGL");
    if (nullptr == window)
    {
        return -1;
    }
    
    //������ɫ���ı���
    const std::string vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position,1.0);\n"
        "}\n";
    
    GLuint vs = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    if (!vs)
    {
        return -2;
    }

    //Ƭ����ɫ���ı���
    const std::string fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    GLuint fs = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    if (!fs)
    {
        return -3;
    }

    //������ɫ������
    GLuint shaderProgram = CreateShaderProgram(vs, fs);
    if (!shaderProgram) {
        return -4;
    }

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
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
        // 3. ���ö�������ָ��
            //(1)����index
            //(2)count
            //(3)���ݻ�������
            //(4)һ���������Ե��ܲ���
            //(5)��ǰ���Ե���ʼƫ��
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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