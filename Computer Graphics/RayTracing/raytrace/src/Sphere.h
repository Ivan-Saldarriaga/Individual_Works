#pragma once
#include "Material.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Sphere
{
    glm::vec3 center;
    float radius;
    glm::vec3 color;
    glm::vec3 specularVal = glm::vec3(1.0f, 1.0f, 1.0f);
    Material properties;
    bool isGlazed = false;

public:
    Sphere(glm::vec3 center, float radius, glm::vec3 color, bool isGlazed)
    {
        this->center = center;
        this->radius = radius;
        this->color = color;
        this->isGlazed = isGlazed;
    };
    void setOrigin(glm::vec3 newCenter)
    {
        center = newCenter;
    }
    bool raySphereIntersection(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, float &t)
    {
        glm::vec3 oc = rayOrigin - this->center;
        float a = glm::dot(rayDirection, rayDirection);
        float b = 2.0f * glm::dot(oc, rayDirection);
        float c = glm::dot(oc, oc) - this->radius * this->radius;

        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
        {
            return false; // No intersection
        }
        else
        {
            // std::cout << "HIT ";
            //  Compute the two possible solutions for t
            float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
            float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

            if (t1 < t2 && t1 > 0)
            {
                t = t1;
            }
            else
            {
                t = t2;
            }

            if (t <= 0)
            {
                // Both intersections are behind the ray
                return false;
            }
            return true;
        }
    }
    bool getGlazed()
    {
        return isGlazed;
    }
    glm::vec3 getOrigin()
    {
        return center;
    }
    glm::vec3 getAmbientColor()
    {
        // run shading calculations!
        // ambient
        glm::vec3 ambient = properties.ambientCoefficient * color;
        // diffuse

        return ambient;
    }
    glm::vec3 getLambertColor()
    {
        glm::vec3 lambert = properties.diffuseCoeffeiceint * color;
        return lambert;
    }
    glm::vec3 getColor()
    {
        return color;
    }
    glm::vec3 getNormal(glm::vec3 &intersection)
    {
        glm::vec3 normal = glm::normalize(intersection - center);
        return normal;
    }
    glm::vec3 shade(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, float t, Light myLight, Camera myCamera)
    {
        glm::vec3 intersection = rayOrigin + t * rayDirection;
        glm::vec3 normal = getNormal(intersection);
        glm::vec3 viewDirection = glm::normalize(myCamera.getCameraPos() - intersection);
        float lambert = glm::max(0.0f, glm::dot(normal, myLight.getLightDir()));
        glm::vec3 lambertColor = lambert * getLambertColor();
        glm::vec3 bisector = glm::normalize(myLight.getLightDir() + viewDirection);
        float specularColor = glm::pow(glm::max(0.0f, glm::dot(normal, bisector)), properties.n);
        glm::vec3 specularReflection = specularColor * myLight.getLightCol();
        glm::vec3 shadedColor = myLight.getIntensity() * (getAmbientColor() + lambertColor + specularReflection);

        // glm::vec3 shadedColor = myLight.getIntensity() * (lambertColor + specularReflection);
        //  shadedColor = glm::clamp(shadedColor, 0.0f, 1.0f);
        return shadedColor;
        // glm::vec3 viewDirection = glm::normalize()
    }
    glm::vec3 glazedShade(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, float t, Light myLight, Camera myCamera)
    {
        glm::vec3 intersection = rayOrigin + t * rayDirection;
        glm::vec3 normal = getNormal(intersection);
        glm::vec3 viewDirection = glm::normalize(myCamera.getCameraPos() - intersection);
        float lambert = glm::max(0.0f, glm::dot(normal, myLight.getLightDir()));
        glm::vec3 lambertColor = lambert * getLambertColor();
        glm::vec3 shadedColor = myLight.getIntensity() * (getAmbientColor() + lambertColor);
        // glm::vec3 shadedColor = myLight.getIntensity() * (lambertColor);

        return shadedColor;
    }
};