#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Material
{
public:
    float ambientCoefficient = 0.4f;
    float diffuseCoeffeiceint = 0.9f;
    float specularCoefficient = 0.2f;
    int n = 100;
    bool isGlazed;
};