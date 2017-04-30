#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

const GLuint WIDTH = 800, HEIGHT = 600;
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"                         //定了输入变量的位置值
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const GLchar* fragmentShaderSourceBlue = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.0f, 0.0f, 1.f, 1.0f);\n"
"}\n\0";

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

int main()
{
    GLFWwindow* window;
    startWindowBefore(&window);
    
    //1.顶点着色器
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    
    GLint success = 0;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//获取编译的错误
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //2.片段着色器
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//创建之后，用fragmentshadersource绑定上去
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//获取编译的错误
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    GLuint bluefragmentShader;
    bluefragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(bluefragmentShader, 1, &fragmentShaderSourceBlue, NULL);//创建之后，用fragmentshadersource绑定上去
    glCompileShader(bluefragmentShader);
    
    glGetShaderiv(bluefragmentShader, GL_COMPILE_STATUS, &success);//获取编译的错误
    if (!success)
    {
        glGetShaderInfoLog(bluefragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //3.着色器程序
    //着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout << "EROOR:LINKEDFAILED\n" << infoLog << std::endl;
    }
    
    //3.2着色器对象2
    GLuint shaderProgram2;
    shaderProgram2 = glCreateProgram();
    
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, bluefragmentShader);
    glLinkProgram(shaderProgram2);
    
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram2,512,NULL,infoLog);
        std::cout << "EROOR:LINKEDFAILED\n" << infoLog << std::endl;
    }
    
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(bluefragmentShader);
    
    //4.VAO.VBO
    GLfloat firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // Left
        -0.0f, -0.5f, 0.0f,  // Right
        -0.45f, 0.5f, 0.0f,  // Top
    };
    
    GLfloat secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // Left
        0.9f, -0.5f, 0.0f,  // Right
        0.45f, 0.5f, 0.0f   // Top
    };
    
    //4.0 初始化VAO,VBO
    GLuint VBO[2],VAO[2];
    glGenVertexArrays(2,VAO);
    glGenBuffers(2, VBO);
    
    //4.1 第一个三角形
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    //4.1 第二个三角形
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
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
        //Program
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //Program2
        glUseProgram(shaderProgram2);
        
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glBindVertexArray(0);
        
        //交换缓冲
        glfwSwapBuffers(window);
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    
    //内存释放
    glfwTerminate();
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
