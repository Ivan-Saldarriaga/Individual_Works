#pragma once
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include <vector>
#include <glm/glm.hpp>
#include <tuple>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
class Tetrahedron
{
    vector<tuple<glm::vec3, glm::vec3, glm::vec3>> verticies;
    glm::vec3 color;
    glm::vec3 specularVal = glm::vec3(1.0f, 1.0f, 1.0f);
    Material properties;
    bool isGlazed = false;

public:
    // Default constructor
    Tetrahedron()
        : color(glm::vec3(0.0f)), specularVal(glm::vec3(0.0f)), properties(), isGlazed(false)
    {
    }
    Tetrahedron(vector<glm::vec3> verts, glm::vec3 color, bool glazed)
    {
        /// create different triangles
        // triangle 1 verts 0,1,2

        verticies.push_back(make_tuple(verts[0], verts[1], verts[2]));
        // triangle 2 verts 0,2,3
        verticies.push_back(make_tuple(verts[0], verts[2], verts[3]));
        // triangle 3 verts 0, 3, 1
        verticies.push_back(make_tuple(verts[0], verts[3], verts[1]));
        // triangle 4 verts 1,3,2
        verticies.push_back(make_tuple(verts[1], verts[3], verts[2]));
        // this should all be triangles that have outward facing normals on all sides!
        this->color = color;
        isGlazed = glazed;
    };
    bool rayTetrahedronIntersection(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, float &t)
    {
        // must do for all triangles
        bool hit = false;
        float closestT = numeric_limits<float>::infinity();
        // ray triangle intersection brought the help of generative AI with personal edits to make sure it worked for my setup of a tetrahedron... 
        //it follows the principles of the Loller-Trumbore intersection algorithm as seen here: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm 
        //
        for (auto &triangle : verticies)
        {
            glm::vec3 v0 = get<0>(triangle);
            glm::vec3 v1 = get<1>(triangle);
            glm::vec3 v2 = get<2>(triangle);
            // ray triangle intersection
            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 h = glm::cross(rayDirection, edge2);
            //essentially the determinant!
            float a = glm::dot(edge1, h);
            // check if ray is parallel to the triangle
            if (a > -0.00001f && a < 0.00001f)
            {
                continue;
            }
            //inverse determinant
            float f = 1.0f / a;
            glm::vec3 s = rayOrigin - v0;
            float u = f * glm::dot(s, h);
            if (u < 0.0f || u > 1.0f)
            {
                continue; // Intersection point is outside the triangle
            }
            
            glm::vec3 q = glm::cross(s, edge1);
            float v = f * glm::dot(rayDirection, q);
            if (v < 0.0f || u + v > 1.0f)
            {
                continue; // Intersection point is outside the triangle
            }
            float tVal = f * glm::dot(edge2, q);
            if (tVal > 0.00001f)
            {
                if (tVal < closestT)
                {
                    closestT = tVal;
                }
                hit = true;
                // return true; // Ray intersects the triangle
            }
        }
        t = closestT;
        return hit;
    }

    glm::vec3 getNormal(tuple<glm::vec3, glm::vec3, glm::vec3> &triangle)
    {
        glm::vec3 V0 = std::get<0>(triangle);
        glm::vec3 V1 = std::get<1>(triangle);
        glm::vec3 V2 = std::get<2>(triangle);

        // Calculate the edges of the triangle
        glm::vec3 edge1 = V1 - V0;
        glm::vec3 edge2 = V2 - V0;

        // Calculate the normal using the cross product of the edges
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        return normal;
    }
    bool getGlazed()
    {
        return isGlazed;
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
    glm::vec3 normalHelp(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, float &t, Light myLight, Camera myCamera)
    {
        glm::vec3 closestNormal(0.0f);
        float closestT = std::numeric_limits<float>::max(); // Initialize closest intersection distance
        // brought back the intersection code but sicne this needs to find the normal, I couldnt reuse what I did for rayTetrahedronIntersection because it returns a boolean rather than a normal vector
        for (auto &triangle : verticies)
        {
            glm::vec3 v0 = get<0>(triangle);
            glm::vec3 v1 = get<1>(triangle);
            glm::vec3 v2 = get<2>(triangle);
            // ray triangle intersection
            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 h = glm::cross(rayDirection, edge2);
            float a = glm::dot(edge1, h);

            if (a > -0.00001f && a < 0.00001f)
            {
                continue; // Ray is parallel to the triangle
            }
            float f = 1.0f / a;
            glm::vec3 s = rayOrigin - v0;
            float u = f * glm::dot(s, h);
            if (u < 0.0f || u > 1.0f)
            {
                continue; // Intersection point is outside the triangle
            }

            glm::vec3 q = glm::cross(s, edge1);
            float v = f * glm::dot(rayDirection, q);
            if (v < 0.0f || u + v > 1.0f)
            {
                continue; // Intersection point is outside the triangle
            }
            float tVal = f * glm::dot(edge2, q);
            if (tVal > 0.00001f && tVal < closestT)
            {
                closestT = tVal;
                closestNormal = getNormal(triangle);
            }
        }
        return closestNormal;
    }
    glm::vec3 shade(glm::vec3 &rayOrigin, glm::vec3 &rayDirection, float &t, Light myLight, Camera myCamera)
    {
        glm::vec3 intersection = rayOrigin + t * rayDirection;
        // try to get normal at 't' have to find WHICH triangle was hit
        glm::vec3 normal(0.0f);
        // normal = closestNormal;
        normal = normalHelp(rayOrigin, rayDirection, t, myLight, myCamera);
        if (glm::dot(normal, rayDirection) > 0)
        {
            normal *= -1.0f;
        }
        // cout << normal.r << " " << normal.g << " " << normal.b << endl;
        //   SAME AS CIRCLE!
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
    }
};