#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "object.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath> 
#include <vector>
#include <random>
//#include "camera.h"
//////////// DECLERATIONS //////////////
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffest);
//////////// DEFINITIONS ///////////////
//Camera params
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
float lastX = 800, lastY = 600, yaw = -90.0f, pitch = 0, fov = 0;
bool rain = false, firstMouse = true;
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix;
glm::mat4 view;

// settings
const unsigned int SCR_WIDTH = 800 * 2;
const unsigned int SCR_HEIGHT = 600 * 2;
//matrix
glm::vec3 translate = glm::vec3(0.0f);
glm::vec3 scale = glm::vec3(1.0f);
glm::vec3 rotateVec = glm::vec3(0.0f);

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
float randomFloat()
{
    float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / 0.3f);

    return random;
}
 
void fillPoints(vector<glm::vec3>& points){
    if (!parsePoints(points)) {
        std::cerr << "Failed to parse points\n";
    }

    // Calculate the bounding box of the points
    glm::vec3 minPoint = points[0];
    glm::vec3 maxPoint = points[0];

    for (const auto& point : points) {
        if (point.x < minPoint.x) minPoint.x = point.x;
        if (point.y < minPoint.y) minPoint.y = point.y;
        if (point.z < minPoint.z) minPoint.z = point.z;
        if (point.x > maxPoint.x) maxPoint.x = point.x;
        if (point.y > maxPoint.y) maxPoint.y = point.y;
        if (point.z > maxPoint.z) maxPoint.z = point.z;
    }

    // Calculate the range of the bounding box
    float rangeX = maxPoint.x - minPoint.x;
    float rangeY = maxPoint.y - minPoint.y;
    float rangeZ = maxPoint.z - minPoint.z;

    for (auto& point : points) {
        point.x = (point.x - minPoint.x) / rangeX * 2.0f - 1.0f;
        point.y = (point.y - minPoint.y) / rangeY * 2.0f - 1.0f;
        point.z = (point.z - minPoint.z) / rangeZ * 2.0f - 1.0f;
    }
}
void setColorByDistance(float distance, glm::vec3& color){
    if (rain == true){
        float rainChange = randomFloat();
        distance += rainChange;
    }
    if (distance < 2) {
        color = glm::vec3(1.0f, 0.0f, 0.0f); // Red
    } else if (distance < 3) {
        color = glm::vec3(0.0f, 1.0f, 0.0f); // Green
    } else {
        color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue
    }

}
float distance(glm::vec3 p1, glm::vec3 p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float movementSpeed = 0.001f;
    float theta = 0.05f;
    float scaleVal = 0.001f;
    float cameraSpeed = 0.005f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    cameraPos -= glm::normalize(cameraUp) * cameraSpeed; // Move up in the Z direction
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos += glm::normalize(cameraUp) * cameraSpeed;
}

int main() {
    glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 
    // // glew: load all OpenGL function pointers
    glewInit();
    glEnable(GL_DEPTH_TEST);
    // READING SHADERS
    string vertexShaderSrc = readShaders("../shaders/pointCloud.vs");
    string fragmentShaderSrc = readShaders("../shaders/pointCloud.fs");
    //setting up and linking shaders!
    /* #region SHADERS */
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
    
    /* #endregion */
    /*
    // // Initialize GLFW
    // if (!glfwInit()) {
    //     std::cerr << "Failed to initialize GLFW\n";
    //     return -1;
    // }

    // // Create a windowed mode window and its OpenGL context
    // int screenWidth = 800 * 2;
    // int screenHeight = 600 * 2;
    // GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Hello OpenGL", nullptr, nullptr);
    // if (!window) {
    //     std::cerr << "Failed to create GLFW window\n";
    //     glfwTerminate();
    //     return -1;
    // }

    // // Make the window's context current
    // glfwMakeContextCurrent(window);

    // // Initialize GLEW (optional, if using modern OpenGL contexts)
    // GLenum err = glewInit();
    // if (err != GLEW_OK) {
    //     std::cerr << "GLEW initialization failed: " << glewGetErrorString(err) << "\n";
    //     glfwTerminate();
    //     return -1;
    // }
    */
    //////////////////////////////////
    std::vector<glm::vec3> points;
    fillPoints(points);
    std::vector<float> vertexData;
    for (const auto& point : points) {
        vertexData.push_back(point.x);
        vertexData.push_back(point.y);
        vertexData.push_back(point.z);
    }
    glm::vec3 cameraPoint = glm::vec3(0.0f,0.0f,0.0f);
    float aspectRatio = SCR_WIDTH / static_cast<float>(SCR_HEIGHT);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    fov = 45.0f; // Adjust as needed
    /////////////////////////////////////
    glUseProgram(shaderProgram);
    //SEND TO GPU
    /* #region vertex objects*/
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* #endregion*/
    // Rendering loop
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //gpu
        glUseProgram(shaderProgram);
        unsigned int modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
        unsigned int viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        unsigned int projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, points.size());
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    // Clean up GLFW
    glfwTerminate();
    return 0;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}  
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
    //projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}