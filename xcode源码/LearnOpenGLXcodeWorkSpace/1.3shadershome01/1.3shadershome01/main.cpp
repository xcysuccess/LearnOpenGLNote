#include <iostream>
#include <cmath>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;

// Function prototypes
int startWindowBefore(GLFWwindow** outputWindow);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
    GLFWwindow* window;
    startWindowBefore(&window);
    
    //1.顶点着色器
    const GLchar* vertexPath = "/Users/tomxiang/git/xcode源码/LearnOpenGLXcodeWorkSpace/1.3shadershome01/1.3shadershome01/basic.vs";
    const GLchar* fragmentPath = "/Users/tomxiang/git/xcode源码/LearnOpenGLXcodeWorkSpace/1.3shadershome01/1.3shadershome01/basic.frag";
    Shader ourShader(vertexPath,fragmentPath);

    //2.VAO.VBO
    GLfloat vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    
    //2.0 初始化VAO,VBO
    GLuint VBO,VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    
    //2.1 绑定VAO
    glBindVertexArray(VAO);
    
    //2.2 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //2.3 设置顶点属性指针,函数告诉OpenGL该如何解析顶点数据
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));//1颜色属性,3指的是vec3,6代表步长，3*sizeof代表偏移量
    glEnableVertexAttribArray(1);
    
    //2.4 解绑VAO
    glBindVertexArray(0);
    
    
    //让window退出前一直保持运行
    while(!glfwWindowShouldClose(window))
    {
        //检查事件
        glfwPollEvents();
        
        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //绘制物体
        ourShader.Use();
        
        //偏移
        GLfloat offset = 0.5f;
        glUniform1f(glGetUniformLocation(ourShader.Program, "xOffset"), offset);
    
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        //交换缓冲
        glfwSwapBuffers(window);
    }
    
    //内存释放
    glfwTerminate();
    return 0;
}

int startWindowBefore(GLFWwindow** outputWindow){
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    
    //1.实例化GLFW窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    //1.glfwCreateWindow函数需要窗口的宽和高
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    //1.2设置keywindow的回调
    glfwSetKeyCallback(window, key_callback);
    
    //2.需要初始化GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    //3.视口(Viewport)
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    *outputWindow = window;
    
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
