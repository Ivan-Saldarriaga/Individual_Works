#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
    glm::vec3 lightDirection;
    glm::vec3 lightColor;
    float intensity;

public:
    Light(glm::vec3 direction, glm::vec3 color, float intensity)
    {
        lightDirection = glm::normalize(direction);
        lightColor = color;
        this->intensity = intensity;
    }
    float getIntensity()
    {
        return intensity;
    }
    glm::vec3 getLightDir()
    {
        return lightDirection;
    }
    glm::vec3 getLightCol()
    {
        return lightColor;
    }
};