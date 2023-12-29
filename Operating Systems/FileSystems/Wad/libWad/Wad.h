#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <deque>
#include <fstream>
#include <ostream>
#include <istream>
#include <unistd.h>
#include <regex>
#include <unordered_map>
#include <cstring>
#include <string>
#include <algorithm>
#include <cstdint>
using namespace std;
struct Descriptor
{
    string name;
    string filePath;
    uint32_t offset;
    // uint32_t end_offset;
    uint32_t length;
    bool isDirectory;

    vector<Descriptor *> children;
};
class Wad
{
    // function prototypes
public:
    static Wad *loadWad(const string &path);
    string getMagic();
    bool isContent(const string &path);
    bool isDirectory(const string &path);
    int getSize(const string &path);
    int getDirectory(const string &path, vector<string> *directory);
    int getContents(const string &path, char *buffer, int length, int offset = 0);
    void createDirectory(const string &path);
    void createFile(const string &path);
    int writeToFile(const string &path, const char *buffer, int length, int offset = 0);
    int descOff;
    string pathMaker(deque<Descriptor *> paths);
    void dfsDirectory(Descriptor *root, fstream &file);

private:
    // saved variables
    string magic;
    uint32_t numDescriptors;
    uint32_t descriptorOffset;
    Descriptor *rootDirectory;
    unordered_map<string, Descriptor *> pathMap;
    string filePath;
    int fillCount;
};