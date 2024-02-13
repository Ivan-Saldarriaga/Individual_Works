#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
    bool isOrthographic = true;
    float distanceD = 3.0f;
    float initPos = 0.0f;
    glm::vec3 cameraUp = glm::vec3(0.0f);
    glm::vec3 cameraView = glm::vec3(0.0f);
    glm::vec3 cameraPosition = glm::vec3(0.0f);
    glm::vec3 u = glm::vec3(0.0f);
    glm::vec3 v = glm::vec3(0.0f);
    glm::vec3 w = glm::vec3(0.0f);
    glm::vec3 e = glm::vec3(0.0f);

public:
    // Constructor to initialize camera parameters
    Camera(glm::vec3 camPos, glm::vec3 camView, glm::vec3 camUp)
        : cameraPosition(camPos), cameraView(camView), cameraUp(camUp)
    {
        // Calculate the basis vectors e, v, u, and w
        glm::vec3 directionVector = cameraPosition - cameraView;
        glm::vec3 wVector = glm::normalize(directionVector);
        w = wVector;
        u = glm::normalize(glm::cross(cameraUp, w));
        v = glm::cross(w, u);
        e = cameraPosition;
        initPos = cameraPosition.b;
    }
    void setPosition(const glm::vec3 &newPos)
    {
        cameraPosition = newPos;

        // cameraUp = glm::normalize(glm::vec3(0.0f, -newPos.g, newPos.b));
        setVals(cameraPosition, cameraView, cameraUp);
    }
    bool getOrientation()
    {
        return isOrthographic;
    }
    void setVals(glm::vec3 &camPos, glm::vec3 &camView, glm::vec3 &camUp)
    {
        cameraPosition = camPos;
        cameraView = camView;
        cameraUp = camUp;
        // Calculate the basis vectors e, v, u, and w
        glm::vec3 directionVector = cameraPosition - cameraView;
        glm::vec3 wVector = glm::normalize(directionVector);
        w = wVector;
        u = glm::normalize(glm::cross(cameraUp, w));
        v = glm::cross(w, u);
        e = cameraPosition;
    }
    void setOrientation()
    {
        isOrthographic = false;
    }
    glm::vec3 getRayOrigin(float u, float v, float height, float width)
    {
        if (isOrthographic)
        {
            return e + (u * this->u) + (v * this->v);
        }
        else
        {
            return cameraPosition;
        }
    }
    glm::vec3 getRayDirection(float u, float v)
    {
        if (isOrthographic)
        {
            return -w;
        }
        else
        {
            return -(distanceD * this->w) + (u * this->u) + (v * this->v);
        }
    }
    glm::vec3 getCameraPos()
    {
        return cameraPosition;
    }
    void changeOrientation()
    {
        isOrthographic = !isOrthographic;
    }
};