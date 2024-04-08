// parsing the FACES of an object
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
struct Vertex
{
    float x;
    float y;
    float z;
};
struct Triangle
{
    Vertex v1;
    Vertex v2;
    Vertex v3;
    // normals
    Vertex n1;
    Vertex n2;
    Vertex n3;
};
std::map<int, Vertex> parseVerts(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return {};
    }
    map<int, Vertex> verticies;
    string line;
    int vertexCount = 1;
    while (getline(file, line))
    {
        if (line.substr(0, 2) == "v ")
        {
            istringstream iss(line.substr(2)); // Skip "v "
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            // Normalize vertex coordinates
            glm::vec3 v(vertex.x, vertex.y, vertex.z);
            verticies[vertexCount] = vertex;
            vertexCount++;
        }
    }
    file.close();
    return verticies;
}
std::map<int, Vertex> parseNormals(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return {};
    }
    map<int, Vertex> normals;
    string line;
    int normalCount = 1;
    while (getline(file, line))
    {
        // add normal reading
        if (line.substr(0, 3) == "vn ")
        {
            istringstream iss(line.substr(3));
            Vertex normal;
            iss >> normal.x >> normal.y >> normal.z;
            // Normalize normals
            glm::vec3 v(normal.x, normal.y, normal.z);
            normals[normalCount] = normal;
            normalCount++;
            // cerr << normal.x << " ";
        }
    }
    file.close();
    return normals;
}
std::vector<Triangle> parseOBJ(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file: " << filename << endl;
        return {}; // Return an empty vector to indicate failure
    }
    vector<Triangle> triangles;
    map<int, Vertex> vertices = parseVerts(filename);
    map<int, Vertex> normals = parseNormals(filename);
    string line;
    while (getline(file, line))
    {
        if (line.substr(0, 2) == "f ")
        {
            std::istringstream iss(line.substr(2)); // Skip "f "
            std::vector<std::string> vertexIndices;
            std::vector<std::string> normalIndices;
            std::string faceIndex;
            std::string vertexIndex;
            while (iss >> faceIndex)
            {
                std::istringstream indexStream(faceIndex);
                std::string vertexIndexStr, normalIndexStr;
                std::getline(indexStream, vertexIndexStr, '/'); // Get vertex index
                std::getline(indexStream, normalIndexStr, '/'); // Get normal index
                std::getline(indexStream, normalIndexStr, '/');
                // cout << "VERT: " << vertexIndexStr << endl;
                vertexIndices.push_back(vertexIndexStr);
                // cout << "NORM: " << normalIndexStr << endl;
                normalIndices.push_back(normalIndexStr);
            }
            if (vertexIndices.size() == 3)
            { // If it's a triangle
                Triangle triangle;
                triangle.v1 = vertices[std::stoi(vertexIndices[0])];
                triangle.v2 = vertices[std::stoi(vertexIndices[1])];
                triangle.v3 = vertices[std::stoi(vertexIndices[2])];
                triangle.n1 = normals[std::stoi(normalIndices[0])];
                triangle.n2 = normals[std::stoi(normalIndices[1])];
                triangle.n3 = normals[std::stoi(normalIndices[2])];

                triangles.push_back(triangle);
            }
            else if (vertexIndices.size() == 4)
            { // If it's a quad, split it into two triangles
                Triangle triangle1, triangle2;
                triangle1.v1 = vertices[std::stoi(vertexIndices[0])];
                triangle1.v2 = vertices[std::stoi(vertexIndices[1])];
                triangle1.v3 = vertices[std::stoi(vertexIndices[2])];

                triangle1.n1 = normals[std::stoi(normalIndices[0])];
                triangle1.n2 = normals[std::stoi(normalIndices[1])];
                triangle1.n3 = normals[std::stoi(normalIndices[2])];
                // cerr << triangle1.v1.x << " : " << triangle1.n1.x << endl;
                triangles.push_back(triangle1);

                triangle2.v1 = vertices[std::stoi(vertexIndices[0])];
                triangle2.v2 = vertices[std::stoi(vertexIndices[2])];
                triangle2.v3 = vertices[std::stoi(vertexIndices[3])];

                triangle2.n1 = normals[std::stoi(normalIndices[0])];
                triangle2.n2 = normals[std::stoi(normalIndices[2])];
                triangle2.n3 = normals[std::stoi(normalIndices[3])];
                triangles.push_back(triangle2);
            }
            else
            {
                // cerr << "Tm not doing n-gons" << endl;
            }
        }
        else
        {
            // std::cout << line << std::endl; // Output non-face lines as they are
        }
    }
    file.close();
    return triangles;
}
