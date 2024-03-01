//parsing the FACES of an object 
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
struct Vertex {
    float x;
    float y;
    float z;
};
struct Triangle{
    Vertex v1;
    Vertex v2;
    Vertex v3;
};
std::map<int,Vertex> parseVerts(const string& filename){
    ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return {}; 
    }
    map<int, Vertex> verticies;
    string line;
    int vertexCount = 1;
    while(getline(file, line)) {
        if(line.substr(0,2) == "v "){
            istringstream iss(line.substr(2)); // Skip "v "
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            verticies[vertexCount] = vertex;
            vertexCount++;
        }
    }
    file.close();
    return verticies;
}
std::vector<Triangle> parseOBJ(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << endl;
        return {}; // Return an empty vector to indicate failure
    }
    vector<Triangle> triangles;
    map<int, Vertex> vertices = parseVerts(filename);
    string line;
    while (getline(file, line)) {
        if (line.substr(0, 2) == "f ") {
             std::istringstream iss(line.substr(2)); // Skip "f "
            std::vector<std::string> vertexIndices;
            std::string vertexIndex;
            while (iss >> vertexIndex) {
                vertexIndices.push_back(vertexIndex);
            }
            if (vertexIndices.size() == 3) { // If it's a triangle
                Triangle triangle;
                triangle.v1 = vertices[std::stoi(vertexIndices[0])];
                triangle.v2 = vertices[std::stoi(vertexIndices[1])];
                triangle.v3 = vertices[std::stoi(vertexIndices[2])];
                triangles.push_back(triangle);
            } else if (vertexIndices.size() == 4) { // If it's a quad, split it into two triangles
                Triangle triangle1, triangle2;
                triangle1.v1 = vertices[std::stoi(vertexIndices[0])];
                triangle1.v2 = vertices[std::stoi(vertexIndices[1])];
                triangle1.v3 = vertices[std::stoi(vertexIndices[2])];
                triangles.push_back(triangle1);
                
                triangle2.v1 = vertices[std::stoi(vertexIndices[0])];
                triangle2.v2 = vertices[std::stoi(vertexIndices[2])];
                triangle2.v3 = vertices[std::stoi(vertexIndices[3])];
                triangles.push_back(triangle2);
            } else {
                std::cerr << "Invalid face definition: " << line << std::endl;
            }
        } else {
            //std::cout << line << std::endl; // Output non-face lines as they are
        }
        
    }
    file.close();
    return triangles;
}
