#pragma once
#include <functional>
#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iomanip>
#include <climits>
using namespace std;

struct MemoryBlock
{
    int startAddress;
    size_t sizeInBytes;
    bool isAllocated;
    MemoryBlock *next;
};
//  g++ MemoryManager.cpp -o output_binary
//>> ./output_binary
class MemoryManager
{
public:
    MemoryManager(unsigned wordSize, function<int(int, void *)> allocator);
    ~MemoryManager();
    void initialize(size_t sizeInWords);
    void shutdown();
    void *allocate(size_t sizeInBytes);
    void free(void *address);
    void setAllocator(function<int(int, void *)> allocator);
    int dumpMemoryMap(char *filename);
    void *getList();
    void *getBitmap();
    unsigned getWordSize();
    void *getMemoryStart();
    unsigned getMemoryLimit();

    void printAll();

private:
    unsigned defaultWordSize;
    function<int(int, void *)> defaultAllocator;
    unsigned totalMemorySize;
    uint8_t *memoryBlock = nullptr;
    MemoryBlock *memoryBlockList;
    int holesArraySize = 0;
};
int bestFit(int sizeInWords, void *list);
int worstFit(int sizeInWords, void *list);