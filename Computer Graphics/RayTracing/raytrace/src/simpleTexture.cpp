// Based on templates from learnopengl.com
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include "Camera.h"
#include "Sphere.h"
#include "Light.h"
#include "Tetrahedron.h"
// FOR MOVIE
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
// using namespace std;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
glm::vec3 camPos(0.0f, 0.0f, 10.0f);
glm::vec3 camView(0.0f, 0.0f, 0.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
Camera myCamera(camPos, camView, camUp);
// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "layout (location = 2) in vec2 aTexCoord;\n"
                                 "out vec3 ourColor;\n"
                                 "out vec2 TexCoord;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(aPos, 1.0);\n"
                                 "ourColor = aColor;\n"
                                 "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "uniform sampler2D texture1;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = texture(texture1, TexCoord);\n"
                                   "}\n\0";
glm::vec3 mirrorRecursion(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, int depth, vector<Sphere> &spheres, Tetrahedron &tetrahedron, Light &myLight, Camera &myCamera)
{
    if (depth <= 0)
    {
        // cout << "Reached recursion depth limit" << endl;
        return glm::vec3(0.0f);
    }

    // Find the closest intersection
    float closestT = numeric_limits<float>::infinity();
    Sphere closestSphere(glm::vec3(0.0f), 0.0f, glm::vec3(0.0f), false);
    bool isSphere = true;
    for (auto &sphere : spheres)
    {
        float t;
        if (sphere.raySphereIntersection(rayOrigin, rayDirection, t) && t < closestT)
        {
            closestT = t;
            closestSphere = sphere;
        }
    }
    if (true)
    {
        float t;
        if (tetrahedron.rayTetrahedronIntersection(rayOrigin, rayDirection, t) && t < closestT)
        {
            closestT = t;
            isSphere = false;
        }
    }

    if (closestT == numeric_limits<float>::infinity())
    {
        // cout << "No intersection found" << endl;
        return glm::vec3(0.0f);
    }

    // cout << "Intersection found" << endl;
    if (isSphere)
    {
        // Compute reflected ray, origin is intersection
        glm::vec3 intersection = rayOrigin + closestT * rayDirection;
        glm::vec3 normal = closestSphere.getNormal(intersection);
        glm::vec3 reflectedRayOrigin = intersection + 0.001f * normal;
        glm::vec3 reflectedRayDirection = rayDirection - 2.0f * glm::dot(rayDirection, normal) * normal;

        // std::cout << closestSphere.getGlazed() << std::endl;
        glm::vec3 reflectionColor = mirrorRecursion(reflectedRayOrigin, reflectedRayDirection, depth - 1, spheres, tetrahedron, myLight, myCamera);
        // returns the color of the OTHER sphere(s) in the direction of the reflection ray
        glm::vec3 shadedColor = closestSphere.shade(rayOrigin, rayDirection, closestT, myLight, myCamera) + reflectionColor;
        // shadedColor = glm::clamp(shadedColor, 0.0f, 1.0f);
        // cout << closestSphere.getGlazed() << endl;
        return shadedColor;
    }
    else
    {
        glm::vec3 intersection = rayOrigin + closestT * rayDirection;
        glm::vec3 normal = tetrahedron.normalHelp(rayOrigin, rayDirection, closestT, myLight, myCamera);
        glm::vec3 reflectedRayOrigin = intersection + 0.001f * normal;
        glm::vec3 reflectedRayDirection = rayDirection - 2.0f * glm::dot(rayDirection, normal) * normal;

        // std::cout << closestSphere.getGlazed() << std::endl;
        glm::vec3 reflectionColor = mirrorRecursion(reflectedRayOrigin, reflectedRayDirection, depth - 1, spheres, tetrahedron, myLight, myCamera);
        // returns the color of the OTHER sphere(s) in the direction of the reflection ray
        glm::vec3 shadedColor = tetrahedron.shade(rayOrigin, rayDirection, closestT, myLight, myCamera) + reflectionColor;

        return shadedColor;
    }
}
bool shadowShading(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, Sphere &currSphere, vector<Sphere> &spheres, Tetrahedron &tetrahedron, Light myLight, Camera myCamera, bool istetra)
{
    // Find the closest intersection
    float closestT = numeric_limits<float>::infinity();
    Sphere closestSphere(glm::vec3(0.0f), 0.0f, glm::vec3(0.0f), true);
    bool isSphere = true;
    for (auto &sphere : spheres)
    {
        float t;
        if (sphere.raySphereIntersection(rayOrigin, rayDirection, t) && t < closestT && sphere.getOrigin() != currSphere.getOrigin())
        {
            closestT = t;
            closestSphere = sphere;
        }
    }

    if (!istetra)
    {
        float t;
        if (tetrahedron.rayTetrahedronIntersection(rayOrigin, rayDirection, t) && t < closestT)
        {
            closestT = t;
            isSphere = false;
        }
    }
    if (closestT == numeric_limits<float>::infinity())
    {
        // cout << "No intersection found" << endl;
        return false;
    }
    // intersection found, shadow reutrned
    return true;
}
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Display RGB Array", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    // build and compile the shaders
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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
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
    float vertices[] = {
        // positions          // colors           // texture coords
        // 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        // 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        // -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        // -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create the image(RGB Array) to be displayed
    const int width = 512 / 2;  // keep it in powers of 2!
    const int height = 512 / 2; // keep it in powers of 2!
    unsigned char *image = new unsigned char[width * height * 3];
    // instanciate obejcts in scene!
    vector<Sphere> spheres;
    spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, 0.4f), 0.3f, glm::vec3(0.63f, 0.40f, 0.60f), false));
    spheres.push_back(Sphere(glm::vec3(0.0f, -1.0f, 0.0f), 0.6f, glm::vec3(0.56f, 0.37f, 0.25f), true));
    spheres.push_back(Sphere(glm::vec3(0.4f, -0.3f, 0.5f), 0.1f, glm::vec3(0.4f, 0.2f, 0.8f), false));
    // cout << "Please add your own sphere, start with its center coordinates!" << endl;

    vector<glm::vec3> tetrahedronVertices;
    // Define the vertices of the tetrahedron
    tetrahedronVertices.push_back(glm::vec3(0.5f, 0.5f, -0.7f));

    tetrahedronVertices.push_back(glm::vec3(0.0f, 1.0f, -0.5f));
    tetrahedronVertices.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
    tetrahedronVertices.push_back(glm::vec3(-0.5f, 0.7f, -0.5f));

    Tetrahedron tetrahedron(tetrahedronVertices, glm::vec3(0.83f, 0.2f, 0.13f), true);

    // SHADING PREFAB
    vector<Light> lights;

    lights.push_back(Light(glm::vec3(0.8f, 0.8f, 0.5f), glm::vec3(0.25f, 0.91f, 0.97f), 0.6f));
    lights.push_back(Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.9f, 0.29f, 0.95f), 0.7f));

    // ORTHO
    bool pressed = false;
    float angle = 0.0f;
    bool moveCamera = false;
    int frameCounter = 0;
    while (true)
    {
        // cout << "AT TOP" << endl;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                // chat gpt, help me change this

                float u = static_cast<float>(i + 0.5f) / static_cast<float>(height);
                float v = static_cast<float>(j + 0.5f) / static_cast<float>(width);
                u = 2.0f * u - 1.0f; // Map to [-1, 1]
                // cout << u << endl;
                v = 1.0f - 2.0f * v; // Map to [-1, 1]
                // cout << v << endl;

                // compute ray origin and direction
                // std::cout << "[" << u << ", " << v << "]";
                glm::vec3 rayOrigin = myCamera.getRayOrigin(u, v, height, width);
                glm::vec3 rayDirection = myCamera.getRayDirection(u, v);
                // loop through objects and find closest t
                float closestT = numeric_limits<float>::infinity();
                bool isSphere = true;
                Sphere closestSphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), false);

                glm::vec3 closestTColor;
                int idx = (i * width + j) * 3;
                for (auto &sphere : spheres)
                {
                    float t;
                    if (sphere.raySphereIntersection(rayOrigin, rayDirection, t) && t < closestT)
                    {
                        // cout << "hit" << endl;
                        closestT = t;
                        closestSphere = sphere;
                    }
                }
                if (true)
                {
                    float t;
                    if (tetrahedron.rayTetrahedronIntersection(rayOrigin, rayDirection, t) && t <= closestT)
                    {
                        closestT = t;
                        isSphere = false;
                    }
                }
                if (closestT < numeric_limits<float>::infinity())
                {
                    glm::vec3 shadedColor(0.0f);
                    for (auto &currLight : lights)
                    {
                        if (isSphere)
                        {
                            if (!closestSphere.getGlazed())
                            {
                                shadedColor += closestSphere.shade(rayOrigin, rayDirection, closestT, currLight, myCamera);
                            }
                            else
                            {

                                shadedColor += closestSphere.shade(rayOrigin, rayDirection, closestT, currLight, myCamera);
                                glm::vec3 intersection = rayOrigin + closestT * rayDirection;
                                glm::vec3 normal = closestSphere.getNormal(intersection);
                                glm::vec3 reflectedRayOrigin = intersection + 0.01f * normal;
                                glm::vec3 reflectedRayDirection = rayDirection - 2.0f * glm::dot(rayDirection, normal) * normal;

                                glm::vec3 refelctionColor = mirrorRecursion(reflectedRayOrigin, reflectedRayDirection, 1, spheres, tetrahedron, currLight, myCamera);

                                shadedColor += refelctionColor;
                            }
                            // trying shadows- sounds similar to mirror reflections but instead of a reflective ray, it takes a ray from intersection to light source!
                            glm::vec3 intersection = rayOrigin + closestT * rayDirection;
                            glm::vec3 normal = closestSphere.getNormal(intersection);
                            glm::vec3 shadowRayOrigin = intersection + 0.01f * normal;
                            glm::vec3 shadowRayDirection = currLight.getLightDir();

                            if (shadowShading(shadowRayOrigin, shadowRayDirection, closestSphere, spheres, tetrahedron, currLight, myCamera, false))
                            {
                                shadedColor *= 0.8f;
                            }
                        }
                        else
                        {
                            // is tetrahedron
                            if (tetrahedron.getGlazed())
                            {
                                shadedColor += tetrahedron.shade(rayOrigin, rayDirection, closestT, currLight, myCamera);
                                glm::vec3 intersection = rayOrigin + closestT * rayDirection;
                                glm::vec3 normal = tetrahedron.normalHelp(rayOrigin, rayDirection, closestT, currLight, myCamera);
                                glm::vec3 reflectedRayOrigin = intersection + 0.01f * normal;
                                glm::vec3 reflectedRayDirection = rayDirection - 2.0f * glm::dot(rayDirection, normal) * normal;

                                glm::vec3 refelctionColor = mirrorRecursion(reflectedRayOrigin, reflectedRayDirection, 1, spheres, tetrahedron, currLight, myCamera);

                                shadedColor += refelctionColor;
                            }
                            else
                            {
                                shadedColor += tetrahedron.shade(rayOrigin, rayDirection, closestT, currLight, myCamera);
                            }
                            glm::vec3 intersection = rayOrigin + closestT * rayDirection;
                            glm::vec3 normal = tetrahedron.normalHelp(rayOrigin, rayDirection, closestT, currLight, myCamera);
                            glm::vec3 shadowRayOrigin = intersection + 0.01f * normal;
                            glm::vec3 shadowRayDirection = currLight.getLightDir();

                            if (shadowShading(shadowRayOrigin, shadowRayDirection, closestSphere, spheres, tetrahedron, currLight, myCamera, true))
                            {
                                shadedColor *= 0.8f;
                            }
                        }
                    }
                    shadedColor = glm::clamp(shadedColor, 0.0f, 1.0f);
                    image[idx] = static_cast<unsigned char>(shadedColor.r * 255.0f);
                    image[idx + 1] = static_cast<unsigned char>(shadedColor.g * 255.0f);
                    image[idx + 2] = static_cast<unsigned char>(shadedColor.b * 255.0f);
                }
                else
                {
                    // If there's no intersection, set the pixel color to black
                    image[idx] = 0;
                    image[idx + 1] = 0;
                    image[idx + 2] = 0;
                }
            }
        }

        unsigned char *data = &image[0];
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        // render loop
        // -----------

        while (!glfwWindowShouldClose(window))
        {
            // FOR MOVIE
            //  char filename[128];
            //  sprintf(filename, "../src/output2/frame_%04d.png", frameCounter);
            //  stbi_write_png(filename, 1024, 1024, 3, data, 0);

            // // Increment frame counter
            // frameCounter++;

            processInput(window);
            int spaceState = glfwGetKey(window, GLFW_KEY_SPACE);
            if (spaceState == GLFW_PRESS && !pressed)
            {
                pressed = true; // Set the flag to indicate spacebar is pressed
                // std::cout << "Spacebar pressed" << std::endl;
                myCamera.changeOrientation();
                // cout << myCamera.getCameraPos().r << " " << myCamera.getCameraPos().g << " " << myCamera.getCameraPos().b << endl;
                break;
            }
            else if (spaceState == GLFW_RELEASE)
            {
                pressed = false; // Reset the flag when spacebar is released
            }
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !moveCamera)
            {
                // cout << "MOVE CAMERA TRUE" << endl;
                moveCamera = true;
            }
            if (moveCamera)
            {
                // Update camera position based on angle
                float radius = 5.0f;
                float camX = sin(angle) * radius;
                float camY = radius * sin(angle);
                float camZ = cos(angle) * radius;
                spheres[2].setOrigin(glm::vec3(cos(angle), cos(angle), sin(angle)));
                myCamera.setPosition(glm::vec3(camX, camY, camZ));
                angle += 0.01f;
                // cout << myCamera.getCameraPos().r << " " << myCamera.getCameraPos().g << " " << myCamera.getCameraPos().b << endl;
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // Bind Texture
                glBindTexture(GL_TEXTURE_2D, texture);

                // Render container
                glUseProgram(shaderProgram);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                // Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                glfwSwapBuffers(window);
                glfwPollEvents();

                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                {
                    // cout << "STOP ANIMATION" << endl;
                    moveCamera = false;
                }
                break;
            }

            // Render the scene as usual
            // Clear the window
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Bind Texture
            glBindTexture(GL_TEXTURE_2D, texture);

            // Render container
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // cout << "loop?" << endl;

        if (glfwWindowShouldClose(window))
        {
            // cout << "why????" << endl;
            //  optional: de-allocate all resources once they've outlived their purpose:
            //  ------------------------------------------------------------------------
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            // glfw: terminate, clearing all previously allocated GLFW resources.
            // ------------------------------------------------------------------
            glfwTerminate();
            return 0;
        }
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}