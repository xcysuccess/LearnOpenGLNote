#include <iostream>
#include <cmath>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "shader.h"

// Other Libs
#include <SOIL.h>

const GLuint WIDTH = 800, HEIGHT = 600;

// Function prototypes
int startWindowBefore(GLFWwindow** outputWindow);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
    GLFWwindow* window;
    startWindowBefore(&window);
    
    //1.顶点着色器
    const GLchar* vertexPath = "/Users/tomxiang/git/xcode源码/LearnOpenGLXcodeWorkSpace/1.4texturesmulti/1.4texturesmulti/basic.vs";
    const GLchar* fragmentPath = "/Users/tomxiang/git/xcode源码/LearnOpenGLXcodeWorkSpace/1.4texturesmulti/1.4texturesmulti/basic.frag";
    Shader ourShader(vertexPath,fragmentPath);
    
    //2.VAO.VBO
    GLfloat vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    
    //2.0 初始化VAO,VBO
    GLuint VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    //2.1 绑定VAO
    glBindVertexArray(VAO);
    
    //2.2 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //2.3 设置顶点属性指针,函数告诉OpenGL该如何解析顶点数据
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));//1颜色属性,3指的是vec3,6代表步长，3*sizeof代表偏移量
    glEnableVertexAttribArray(1);
    
    //纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6* sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    //2.4 解绑VAO
    glBindVertexArray(0);
    
    //3.1根据贴图创建纹理
    GLuint texture1,texture2;
    
    // ====================
    // Texture 1
    // ====================
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    //3.2设置环绕过滤方式
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //3.3加载并生成纹理
    //第一个参数指定了纹理目标(Target),第二个参数为纹理指定多级渐远纹理的级别,第三个参数告诉OpenGL我们希望把纹理储存为何种格式,第四个和第五个参数设置最终的纹理的宽度和高度,第七第八个参数定义了源图的格式和数据类型
    int width, height;
    const char *imagePath = "/Users/tomxiang/git/xcode源码/resources/textures/container.jpg";
    unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);//会为当前绑定的纹理自动生成所有需要的多级渐远纹理
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    
    // ===================
    // Texture 2
    // ===================
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    //3.2设置环绕过滤方式
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //3.3加载并生成纹理
    //第一个参数指定了纹理目标(Target),第二个参数为纹理指定多级渐远纹理的级别,第三个参数告诉OpenGL我们希望把纹理储存为何种格式,第四个和第五个参数设置最终的纹理的宽度和高度,第七第八个参数定义了源图的格式和数据类型
    int width2, height2;
    const char *imagePath2 = "/Users/tomxiang/git/xcode源码/resources/textures/awesomeface.png";
    unsigned char* image2 = SOIL_load_image(imagePath2, &width2, &height2, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);//会为当前绑定的纹理自动生成所有需要的多级渐远纹理
    SOIL_free_image_data(image2);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
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
        glBindVertexArray(VAO);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
