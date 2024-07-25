// #pragma once

// #define GLEW_STATIC
// #include <GL/glew.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// // Enumeration for Camera movement directions
// enum Camera_Movement {
//     FORWARD,
//     BACKWARD,
//     LEFT,
//     RIGHT
// };
// const float YAW = -90.0f, PITCH = 0.0f, SPEED = 2.5f, SENSITIVITY = 0.1f, ZOOM =45.0f;
// // Camera class declaration
// class Camera {
// public:
//     // Camera Attributes
//     glm::vec3 Position;
//     glm::vec3 Front;
//     glm::vec3 Up;
//     glm::vec3 Right;
//     glm::vec3 WorldUp;
//     float Yaw;
//     float Pitch;
//     float MovementSpeed;
//     float MouseSensitivity;
//     float Zoom;

//     // Constructors
//     Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
//            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
//            float yaw = -90.0f,
//            float pitch = 0.0f);
    
//     Camera(float posX, float posY, float posZ,
//            float upX, float upY, float upZ,
//            float yaw, float pitch);

//     // Returns the view matrix
//     glm::mat4 GetViewMatrix();

//     // Processes keyboard input
//     void ProcessKeyboard(Camera_Movement direction, float deltaTime);

//     // Processes mouse movement input
//     void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

//     // Processes mouse scroll input
//     void ProcessMouseScroll(float yoffset);

// private:
//     // Calculates the front vector from the Camera's (updated) Euler Angles
//     void updateCameraVectors();
// };

