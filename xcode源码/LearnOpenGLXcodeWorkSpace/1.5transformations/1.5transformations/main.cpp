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

//glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexPath = "/Users/tomxiang/git/xcode源码/LearnOpenGLXcodeWorkSpace/1.5transformations/1.5transformations/basic.vs";
const GLchar* fragmentPath = "/Users/tomxiang/git/xcode源码/LearnOpenGLXcodeWorkSpace/1.5transformations/1.5transformations/basic.frag";

const char *imagePath = "/Users/tomxiang/git/xcode源码/resources/textures/container.jpg";
const char *imagePath2 = "/Users/tomxiang/git/xcode源码/resources/textures/awesomeface.png";

// Holds uniform value of texture mix
GLfloat mixValue = 0.2f;

// Function prototypes
int startWindowBefore(GLFWwindow** outputWindow);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
//    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//    glm::mat4 trans;
//    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//    vec = trans * vec;
//    std::cout << vec.x << vec.y << vec.z << std::endl;
    
    GLFWwindow* window;
    startWindowBefore(&window);
    
    //1.顶点着色器

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
//    环绕方式(Wrapping)	描述
//    GL_REPEAT	对纹理的默认行为。重复纹理图像。
//    GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
//    GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
//    GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
    
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
//    GL_CLAMP_TO_BORDER
//    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
//    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    //3.3加载并生成纹理
    //第一个参数指定了纹理目标(Target),第二个参数为纹理指定多级渐远纹理的级别,第三个参数告诉OpenGL我们希望把纹理储存为何种格式,第四个和第五个参数设置最终的纹理的宽度和高度,第七第八个参数定义了源图的格式和数据类型
    int width, height;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //3.3加载并生成纹理
    //第一个参数指定了纹理目标(Target),第二个参数为纹理指定多级渐远纹理的级别,第三个参数告诉OpenGL我们希望把纹理储存为何种格式,第四个和第五个参数设置最终的纹理的宽度和高度,第七第八个参数定义了源图的格式和数据类型
    int width2, height2;
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
        
        // Set current value of uniform mix
        glUniform1f(glGetUniformLocation(ourShader.Program, "mixValue"), mixValue);
        
        // First container=====================
        
        //uniform的位置值
        //第二个参数告诉OpenGL我们将要发送多少个矩阵，这里是1
        //第三个参数询问我们我们是否希望对我们的矩阵进行置换
        //最后一个参数是真正的矩阵数据
        //旋转加缩放
        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(30.f), glm::vec3(0.0, 0.0, 1.0)); //延z轴旋转

        //home01 先旋转，后位移
        //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        //trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(30.f), glm::vec3(0.0, 0.0, 1.0)); //延z轴旋转
        
        GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        
        // Second container=====================
        glm::mat4 trans2;
        trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
        GLfloat scaleAmount = fabs(sin(glfwGetTime()));//因为sin是-1到1，所以为了不颠倒我这里取fabs
        trans2 = glm::scale(trans2, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        
        //home02 先旋转，后位移
        //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        //trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(30.f), glm::vec3(0.0, 0.0, 1.0)); //延z轴旋转
        transformLoc = glGetUniformLocation(ourShader.Program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans2));
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //交换缓冲
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
    
    // Change value of uniform with arrow keys (sets amount of textre mix)
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        mixValue += 0.1f;
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        mixValue -= 0.1f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}
