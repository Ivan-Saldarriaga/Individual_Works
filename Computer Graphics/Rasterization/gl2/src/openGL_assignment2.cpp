
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
#include <chrono>

glm::mat4 projectionMatrix = glm::mat4(0.0f);

glm::vec3 translate = glm::vec3(0.0f);
glm::vec3 scale = glm::vec3(1.0f);
glm::vec3 rotateVec = glm::vec3(0.0f);
std::vector<Triangle> triangles = parseOBJ("../data/cow.obj");

// setup for adding color per vertex
struct Verts
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// const char *vertexShaderSource = "#version 330 core\n"
//                                  "layout (location = 0) in vec3 aPos;\n"
//                                  "layout (location = 1) in vec3 aColor;\n"
//                                  "out vec3 vertexColor;\n"
//                                  "uniform mat4 modelMatrix;"
//                                  "void main()\n"
//                                  "{\n"
//                                  "   gl_Position = modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//                                  "   vertexColor = aColor;\n"
//                                  "}\0";
// const char *fragmentShaderSource = "#version 330 core\n"
//                                    "in vec3 vertexColor;\n"
//                                    "out vec4 FragColor;\n"
//                                    "void main()\n"
//                                    "{\n"
//                                    "   FragColor = vec4(vertexColor, 1.0);\n"
//                                    "}\n\0";

// READING VS AND FS
// https://www.youtube.com/watch?v=L8u7A-3SXyQ&ab_channel=TheHelloWorldGuy
string readShaders(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "FILE NOT FOUND" << endl;
        return "";
    }
    stringstream ss{};
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
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
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    // FOR CPU
    // string vertexShaderSrc = readShaders("../shaders/source_cpu.vs");
    // string fragmentShaderSrc = readShaders("../shaders/source_cpu.fs");
    string vertexShaderSrc = readShaders("../shaders/source.vs");
    string fragmentShaderSrc = readShaders("../shaders/source.fs");

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // CHANGE FOR READING FROM FILE, must be char*
    const char *vertexShaderSource = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // CHANGE FOR READING FROM FILE
    const char *fragmentShaderSource = fragmentShaderSrc.c_str();

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float fov = 60.0f; // Field of view
    float aspectRatio = (static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT));
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f); // Camera is at (0, 0, 3)
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);   // Camera is looking at the origin
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       // Up direction is the positive Y-axis

    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
    // After linking the shader program, get the location of the uniform variables
    // Light
    glm::vec3 lightPos(1.0f);
    //
    vector<Verts> vertices;
    int vertexIndex = 0;

    for (const auto &triangle : triangles)
    {

        Verts v1, v2, v3;
        v1.position = glm::vec3(triangle.v1.x, triangle.v1.y, triangle.v1.z);
        v1.color = glm::vec3(1.0f, 0.0f, 0.0f);

        v2.position = glm::vec3(triangle.v2.x, triangle.v2.y, triangle.v2.z);
        v2.color = glm::vec3(1.0f, 0.0f, 0.0f);

        v3.position = glm::vec3(triangle.v3.x, triangle.v3.y, triangle.v3.z);
        v3.color = glm::vec3(1.0f, 0.0f, 0.0f);

        v1.normal = glm::vec3(triangle.n1.x, triangle.n1.y, triangle.n1.z);
        v2.normal = glm::vec3(triangle.n2.x, triangle.n2.y, triangle.n2.z);
        v3.normal = glm::vec3(triangle.n3.x, triangle.n3.y, triangle.n3.z);
        //////////////////////////////////////////////////////////////////
        /*
        Comment in the faceNormal Code for flat shading!
        */
        //////////////////////////////////////////////////////////////////

        // glm::vec3 faceNormal;
        // faceNormal.x = (triangle.n1.x + triangle.n2.x + triangle.n3.x) / 3.0f;
        // faceNormal.y = (triangle.n1.y + triangle.n2.y + triangle.n3.y) / 3.0f;
        // faceNormal.z = (triangle.n1.z + triangle.n2.z + triangle.n3.z) / 3.0f;
        // v1.normal = faceNormal;
        // v2.normal = faceNormal;
        // v3.normal = faceNormal;

        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }

    // glEnable(GL_DEPTH_TEST);
    //  MODEL TRANSFORMATION?!
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, translate);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z axis
    float maxCoordinate = 0.0f;
    for (const auto &vertex : vertices)
    {
        float maxCoord = glm::max(glm::max(glm::abs(vertex.position.x), glm::abs(vertex.position.y)), glm::abs(vertex.position.z));
        maxCoordinate = glm::max(maxCoordinate, maxCoord);
    }

    // Set the initial scale vector based on the maximum absolute coordinate value
    glm::vec3 initialScale = glm::vec3(1.0f / maxCoordinate);
    scale = initialScale;
    modelMatrix = glm::scale(modelMatrix, scale);

    // CPU:
    //  for (auto &vertex : vertices)    {
    //      // Apply transformations to vertex positions
    //      vertex.position = glm::vec3(modelMatrix * glm::vec4(vertex.position, 1.0f));
    //  }
    // GPU:
    unsigned int modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // pt3
    unsigned int viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    unsigned int projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");

    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));
    //
    unsigned int numVertices = vertices.size();
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Verts), &vertices[0], GL_STATIC_DRAW);
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Verts), (void *)offsetof(Verts, position));
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verts), (void *)offsetof(Verts, color));
    glEnableVertexAttribArray(1);
    // normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Verts), (void *)offsetof(Verts, normal));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // render loop
    // -----------

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << "Number of Triangles: " << triangles.size() << endl;
    std::cout << "Initialization took " << duration.count() << " seconds" << std::endl;
    // FOR CPU
    // scale = glm::vec3(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glm::mat4 modelMatrix = glm::mat4(1.0f); // Initialize as identity matrix
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z axis
        modelMatrix = glm::scale(modelMatrix, scale);
        // FOR CPU
        //  if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        //  {
        //      // Update vertex data only when scaling keys are pressed
        //      glm::mat4 modelMatrix = glm::mat4(1.0f); // Initialize as identity matrix

        //     modelMatrix = glm::translate(modelMatrix, translate);
        //     modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        //     modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
        //     modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateVec.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z axis
        //     modelMatrix = glm::scale(modelMatrix, scale);
        //     for (auto &vertex : vertices)
        //     {
        //         // Apply transformations to vertex positions
        //         vertex.position = glm::vec3(modelMatrix * glm::vec4(vertex.position, 1.0f));
        //     }
        //     // reset all values or it will save it
        //     translate = glm::vec3(0.0f);
        //     scale = glm::vec3(1.0f);
        //     rotateVec = glm::vec3(0.0f);
        // }
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Verts), &vertices[0]);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // must clear the depth buffer bit added (this fixes Z buffer stuff according to professor!)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        // GPU
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

    float movementVal = 0.001f;
    float rotateVal = 0.05f;
    float scaleVal = 0.001f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        translate.y += movementVal;
        if (translate.y > 1.0f)
        {
            translate.y = 1.0f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        translate.y -= movementVal;
        if (translate.y < -1.0f)
        {
            translate.y = -1.0f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        translate.x -= movementVal;
        if (translate.x < -1.0f)
        {
            translate.x = -1.0f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        translate.x += movementVal;
        if (translate.x > 1.0f)
        {
            translate.x = 1.0f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        translate.z -= movementVal;
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        translate.z += movementVal;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        rotateVec.y += rotateVal;
        rotateVec.x += rotateVal;
        rotateVec.z += rotateVal;
    }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        scale.x += scaleVal;
        scale.y += scaleVal;
        scale.z += scaleVal;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        scale.x -= scaleVal;
        scale.y -= scaleVal;
        scale.z -= scaleVal;
        if (scale.x < 0.0f)
        {
            scale = glm::vec3(0.0001f);
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
