#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <string>
#include <fstream>
#include <iostream>

std::string readFile(char* filename)
{
    std::ifstream ifs(filename);
    if (!ifs)
    {
        std::cout << "file doesn't exist" << std::endl;
        return nullptr;
    }
    std::string content((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));

    return content;
}


bool CompileShader(char* filename, GLuint shader, std::string& log, int logSize = 512)
{
    std::string source = readFile(filename);

    const char * csource = source.c_str();

    glShaderSource(shader, 1, &csource, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    char* buffer = new char[logSize];
    glGetShaderInfoLog(shader, logSize, NULL, buffer);
    log = buffer;

    return status == GL_TRUE;
}

/*
void MainLoop(GLFWwindow* window)
{

}
*/


int main()
{

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    float xSize = 800;
    float ySize = 600;
    GLFWwindow* window = glfwCreateWindow(xSize, ySize, "OpenGL", nullptr, nullptr); // Windowed
    //GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen

    glfwMakeContextCurrent(window);


    glewExperimental = GL_TRUE;
    glewInit();

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    printf("%u\n", vertexBuffer);

    float vertices[] = {
        0.0f, 0.5f, // Vertex 1 (X, Y)
        0.5f, -0.5f, // Vertex 2 (X, Y)
        -0.5f, -0.5f  // Vertex 3 (X, Y)
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char* vertexSource = "basic.vert";
    std::string vertLog;
    CompileShader(vertexSource, vertexShader, vertLog);
    std::cout << vertLog << std::endl;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char* fragmentSource = "basic.frag";
    std::string fragLog;
    CompileShader(fragmentSource, fragmentShader, fragLog);
    std::cout << fragLog << std::endl;

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);


    GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

    GLint windowSize = glGetUniformLocation(shaderProgram, "windowSize");
    glUniform2f(windowSize, xSize, ySize);

    GLint sinTime = glGetUniformLocation(shaderProgram, "sinTime");

    while (!glfwWindowShouldClose(window))
    {

        float time = (float)clock() / (float)CLOCKS_PER_SEC;
        glUniform1f(sinTime, sin(time * 4.0f));
        //float val = ((sin(time * 5.0f) + 1.0f) / 2.0f);
        //glUniform3f(uniColor, val, 0.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

