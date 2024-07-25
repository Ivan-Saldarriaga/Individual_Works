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
//////////// DEFINITIONS ///////////////

bool rain = false;
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

    // Normalize points to fit within the viewport
    int screenWidth = 800;
    int screenHeight = 600;
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
    if (distance < 0.5) {
        color = glm::vec3(1.0f, 0.0f, 0.0f); // Red
    } else if (distance < 1.0) {
        color = glm::vec3(0.0f, 1.0f, 0.0f); // Green
    } else {
        color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue
    }

}
float distance(glm::vec3 p1, glm::vec3 p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

void processInput(GLFWwindow *window, vector<glm::vec3>& points)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float movementVal = 0.001f;
    float theta = 0.05f;
    float scaleVal = 0.001f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        for(auto& point: points){
            point.y += movementVal;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        for(auto& point: points){
            point.y -= movementVal;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        for(auto& point: points){
            point.x -= movementVal;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        for(auto& point: points){
            point.x += movementVal;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        for(auto& point: points){
            point.z += movementVal;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        for(auto& point: points){
            point.z -= movementVal;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        for(auto& point: points){
            //rotate about x
            float newY = point.y * cos(theta) - point.z * sin(theta);
            float newZ = point.y * sin(theta) + point.z * cos(theta);
            point.y = newY;
            point.z = newZ;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        for(auto& point: points){
            //rotate about y
            float newX = point.x * cos(theta) + point.z * sin(theta);
            float newZ = -point.x * sin(theta) + point.z * cos(theta);
            point.x = newX;
            point.z = newZ;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        for(auto& point: points){
            //rotate about z
            float newX = point.x * cos(theta) - point.y * sin(theta);
            float newY = point.x * sin(theta) + point.y * cos(theta);
            point.x = newX;
            point.y = newY;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        rain = true;
        std::cout << rain << std::endl;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    int screenWidth = 800;
    int screenHeight = 600;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW (optional, if using modern OpenGL contexts)
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return -1;
    }
    //////////////////////////////////
    std::vector<glm::vec3> points;
    fillPoints(points);
    glm::vec3 cameraPoint = glm::vec3(0.0f,0.0f,0.0f);
    /////////////////////////////////////

    // Rendering loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window, points);

        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // Draw points
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f); // Red color

        // Draw the specified points
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.7f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glColor3f(1.0f,1.0f,1.0f);
        for(const auto& point : points){
            float dist = distance(point, cameraPoint);
            glm::vec3 color;
            setColorByDistance(dist, color);
            glColor3f(color.r, color.g, color.b);
            glVertex3f(point.x, point.y, point.z);
        }
        glEnd();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up GLFW
    glfwTerminate();
    return 0;
}