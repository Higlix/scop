#include "glad.h"
#include "glfw3.h"

#include "Shader/Shader.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return (-1);
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    
    int  success;
    char infoLog[512];

    /*
        VERTEX SHADER HERE
    */
    Shader shader("shaders/vertex/vertex.vert", "shaders/fragment/fragment.frag");

    // float vertices[] = {
    //     0.5f,  0.5f, 0.0f,  // top right
    //     0.5f, -0.5f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  // bottom left
    //     -0.5f,  0.5f, 0.0f   // top left 
    // };
    // unsigned int indices[] = {  // note that we start from 0!
    //     0, 1, 3,   // first triangle
    //     1, 2, 3    // second triangle
    // };
    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    //unsigned int EBO;

    unsigned int VBO; // vertex buffer object
    unsigned int VAO; // vertex array object 
    glGenVertexArrays(1, &VAO); // generate vertex array object
    glGenBuffers(1, &VBO); // generate vertex buffer object
    //glGenBuffers(1, &EBO); // generate element buffer object 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind to GL_ARRAY_BUFFER 
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind to GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy vertex data to vertex buffer
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1); 
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
    glBindVertexArray(0); // unbind VAO

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // TO DRAW IN LINES
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Input here */
        processInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.ID);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        


        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader.ID);

    glfwTerminate();
    return 0;
}