
// template based on material from learnopengl.com
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "object.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 translate = glm::vec3(0.0f);
glm::vec3 scale = glm::vec3(1.0f);
glm::vec3 rotate = glm::vec3(0.0f);
    
//setup for adding color per vertex
struct Verts {
  glm::vec3 position;
  glm::vec3 color;  
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "uniform mat4 modelMatrix;"
    "void main()\n"
    "{\n"
    "   gl_Position = modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   vertexColor = aColor;\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(vertexColor, 1.0);\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // float vertices[] = {
    //     -0.5f, -.25f, 0.0f, // left  
    //      0.5f, -.75f, 0.0f, // right 
    //      0.0f,  0.5f, 0.0f, // top   

    //     -0.5f, -0.5f, 0.0f, // left  
    //      0.5f, -0.5f, 0.0f, // right 
    //      0.0f, -1.0f, 0.0f  // bottom
    // };
    //cout << "hui" << endl;
    std::vector<Triangle> triangles = parseOBJ("../data/shark.obj");
    vector<Verts> vertices;
    int vertexIndex = 0;
    for (const auto& triangle : triangles) {
        Verts v1, v2, v3;
        v1.position = glm::vec3(triangle.v1.x / 4, triangle.v1.y / 4, triangle.v1.z / 4);
        v1.color = glm::vec3(1.0f, 0.0f, 0.0f);

        v2.position = glm::vec3(triangle.v2.x / 4, triangle.v2.y / 4, triangle.v2.z / 4);
        v2.color = glm::vec3(0.0f, 1.0f, 0.0f);

        v3.position = glm::vec3(triangle.v3.x / 4, triangle.v3.y / 4, triangle.v3.z / 4);
        v3.color = glm::vec3(0.0f, 0.0f, 1.0f); 

        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }

    // MODEL TRANSFORMATION?!
    glm::mat4 modelMatrix = glm::mat4(1.0f);
   
    modelMatrix = glm::translate(modelMatrix, translate);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z axis
    modelMatrix = glm::scale(modelMatrix, scale);
    unsigned int modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        /*
        // Vertex 1
        vertices[vertexIndex++] = triangle.v1.x / 4;
        vertices[vertexIndex++] = triangle.v1.y / 4;
        vertices[vertexIndex++] = triangle.v1.z / 4;

        // Vertex 2
        vertices[vertexIndex++] = triangle.v2.x / 4;
        vertices[vertexIndex++] = triangle.v2.y / 4;
        vertices[vertexIndex++] = triangle.v2.z / 4;

        // Vertex 3
        vertices[vertexIndex++] = triangle.v3.x / 4;
        vertices[vertexIndex++] = triangle.v3.y / 4;
        vertices[vertexIndex++] = triangle.v3.z / 4;
        */
        //cout << "Triangle vertices: [" << triangle.v1.x <<", " << triangle.v1.y << ", " << triangle.v1.z << "] , [" << triangle.v2.x <<", " << triangle.v2.y << ", " << triangle.v2.z << "] , [" << triangle.v3.x <<", " << triangle.v3.y << ", " << triangle.v3.z << "]" << endl;

    //unsigned int numVertices = sizeof(vertices)/sizeof(float)/3;
    unsigned int numVertices = vertices.size();
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Verts), &vertices[0], GL_STATIC_DRAW);
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Verts), (void*)offsetof(Verts, position));
    glEnableVertexAttribArray(0);
    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verts), (void*)offsetof(Verts, color));
    glEnableVertexAttribArray(1);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // std::cout << "hello" << endl;
    // std::vector<Triangle> triangles = parseOBJ("../data/cube.obj");
    // std::cout << "read?" << endl;
    // // Output the triangle vertices
    // for (const auto& triangle : triangles) {
    //     std::cout << "Triangle vertices: [" << triangle.v1.x <<", " << triangle.v1.y << ", " << triangle.v1.z << "] , [" << triangle.v2.x <<", " << triangle.v2.y << ", " << triangle.v2.z << "] , [" << triangle.v3.x <<", " << triangle.v3.y << ", " << triangle.v3.z << "]" << endl;
    // }
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        glm::mat4 modelMatrix = glm::mat4(1.0f); // Initialize as identity matrix
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z axis
        modelMatrix = glm::scale(modelMatrix, scale);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float movementVal = 0.01f;
    float rotateVal = 0.5f;
    float scaleVal = 0.01f;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        translate.y += movementVal;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        translate.y -= movementVal;
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        translate.x -= movementVal;
    }
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        translate.x += movementVal;
    }
    else if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        rotate.y += rotateVal;
        rotate.x += rotateVal;
        rotate.z += rotateVal;
    }
    else if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        scale.x += scaleVal / 4;
        scale.y += scaleVal / 4;
        scale.z += scaleVal / 4;
    }
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        scale.x -= scaleVal;
        scale.y -= scaleVal;
        scale.z -= scaleVal;
        if(scale.x <= 0){
            scale.x = 0.01;
            scale.y = 0.01;
            scale.z = 0.01;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
