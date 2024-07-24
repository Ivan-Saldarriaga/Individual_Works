#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

using namespace std;

bool parsePoints(vector<glm::vec3>& points)
{
    const string& filename = "points.txt";
    ifstream file("../shaders/points.txt");
    if (!file.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return false;
    }
    cout << "READ THE FILE!!!" << endl;
    string line;
    getline(file,line);
    while (getline(file, line))
    {

        istringstream iss(line);
        string token;
        std::string::size_type sz;
        glm::vec3 point;

        // Read x coordinate
        getline(iss, token, ',');
        point.x = stof(token, &sz);

        // Read y coordinate
        getline(iss, token, ',');
        point.y = stof(token, &sz);

        // Read z coordinate
        getline(iss, token, ',');
        point.z = stof(token, &sz);

        points.push_back(point);
    }

    file.close();
    return true;
    //SHOULD BE WORKING
}
