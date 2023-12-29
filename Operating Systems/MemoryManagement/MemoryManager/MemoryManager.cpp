#include "MemoryManager.h"
MemoryManager::MemoryManager(unsigned wordSize, function<int(int, void *)> allocator)
{
    defaultWordSize = wordSize;
    defaultAllocator = allocator;
}
MemoryManager::~MemoryManager()
{
    shutdown();
}

void MemoryManager::initialize(size_t sizeInWords)
{
    // if larger than 65536 words, do nothing
    if (sizeInWords > 65536)
    {
        return;
    }
    // cleanup preious block if applicable
    if (memoryBlock)
    {
        shutdown();
    }
    // Allocate memory block?
    // total size is num words * size of each word (getWordSize?!)
    totalMemorySize = sizeInWords * defaultWordSize;
    //
    intptr_t brk = reinterpret_cast<intptr_t>(sbrk(0));
    sbrk(totalMemorySize);
    memoryBlock = reinterpret_cast<uint8_t*>(brk);
    //memoryBlock = new uint8_t[totalMemorySize];

    // current initialization process
    MemoryBlock *block = new MemoryBlock();
    block->startAddress = 0;
    block->sizeInBytes = totalMemorySize;
    block->isAllocated = false;
    block->next = nullptr;

    memoryBlockList = block;
}

void MemoryManager::shutdown()
{
    if (memoryBlock)
    {
        //delete[] memoryBlock;
        sbrk(-static_cast<intptr_t>(totalMemorySize));
        memoryBlock = nullptr;
    }

    // Free the memory allocated for memoryBlockList
    MemoryBlock *currBlock = memoryBlockList;
    while (currBlock)
    {
        MemoryBlock *temp = currBlock;
        currBlock = currBlock->next;
        delete temp;
    }
    memoryBlockList = nullptr;
}

unsigned MemoryManager::getWordSize()
{
    // if not initiliazed reutrn -1;
    if(!memoryBlock){
        return -1;
    }
    return defaultWordSize;
}

void *MemoryManager::getMemoryStart()
{
    if(!memoryBlock){
        return nullptr;
    }
    return memoryBlock;
}

unsigned MemoryManager::getMemoryLimit()
{
    if(!memoryBlock){
        return -1;
    }
    return totalMemorySize;
}

void MemoryManager::printAll()
{
    MemoryBlock *currBlock = memoryBlockList;

    while (currBlock)
    {
        cout << "[ " << currBlock->isAllocated << " " << currBlock->startAddress / defaultWordSize << " " << currBlock->sizeInBytes / defaultWordSize << " ]" << endl;
        currBlock = currBlock->next;
    }
}

void *MemoryManager::allocate(size_t sizeInBytes)
{
    if(!memoryBlock){
        return nullptr;
    }
    if(sizeInBytes == 0 || sizeInBytes > totalMemorySize){
        return nullptr;
    }
    size_t numWords = (sizeInBytes + defaultWordSize - 1) / defaultWordSize;
    //cout << "NUM WORDS: " << numWords << endl;
    MemoryBlock *currBlock = memoryBlockList;
    //getList allocation

    void* getListVal = (getList());
    int index = defaultAllocator(numWords, reinterpret_cast<uint16_t*>(getListVal));
    delete[] reinterpret_cast<uint16_t*>(getListVal);
    while (currBlock)
    {
        // whatever if statement to narrow down the block
        if (currBlock->startAddress / defaultWordSize == index)
        {
            currBlock->isAllocated = true;
            if (currBlock->sizeInBytes > sizeInBytes)
            {
                //CHANGED FROM SIZEINBYTES TO NUMWORDS!
                MemoryBlock *newBlock = new MemoryBlock();
                newBlock->startAddress = currBlock->startAddress + (numWords * defaultWordSize);
                newBlock->sizeInBytes = currBlock->sizeInBytes - (numWords * defaultWordSize);
                newBlock->isAllocated = false;
                // update pointers
                newBlock->next = currBlock->next;
                currBlock->next = newBlock;
                // update size in bytes of the og
                currBlock->sizeInBytes = numWords * defaultWordSize;
            }
            return &memoryBlock[currBlock->startAddress];
        }
        currBlock = currBlock->next;
    }
    return nullptr;
}

void MemoryManager::free(void *address)
{
    if(!memoryBlock){
        return;
    }
    MemoryBlock *currBlock = memoryBlockList;
    MemoryBlock *prevBlock = nullptr;
    while (currBlock)
    {
        // find matching address
        if (address == &memoryBlock[currBlock->startAddress])
        {
            // Mark the block as unallocated
            currBlock->isAllocated = false;

            // Attempt to merge with the previous block if it is also unallocated
            if (prevBlock && !prevBlock->isAllocated)
            {
                // Merge with the previous block
                prevBlock->sizeInBytes += currBlock->sizeInBytes;
                prevBlock->next = currBlock->next;
                delete currBlock; // Free the memory of the current block
                currBlock = prevBlock;
            }

            // Attempt to merge with the next block if it is also unallocated
            if (currBlock->next && !currBlock->next->isAllocated)
            {
                // Merge with the next block
                currBlock->sizeInBytes += currBlock->next->sizeInBytes;
                MemoryBlock *temp = currBlock->next;
                currBlock->next = temp->next;
                delete temp; // Free the memory of the next block
            }

            return; // Exit the loop once you've found and freed the block.
        }
        prevBlock = currBlock;
        currBlock = currBlock->next;
    }
}

void MemoryManager::setAllocator(function<int(int, void *)> allocator)
{
    defaultAllocator = allocator;
}

int MemoryManager::dumpMemoryMap(char *filename)
{
    if(!memoryBlock){
        return -1;
    }
    // get hole list
    void* getListVal = getList();
    uint16_t* holes = reinterpret_cast<uint16_t*>(getListVal);
    
    // holes[0] is number of holes
    size_t num_holes = static_cast<size_t>(holes[0]);
    // Open the file for writing, create it if it doesn't exist, and truncate it to 0
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777);

    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }
    size_t i;
    for (size_t i = 1; i <= (num_holes * 2); i += 2) {
        
        // Read the start and length of the current hole
        uint16_t start = holes[i];
        uint16_t length = holes[i + 1];
        dprintf(fd, "[%u, %u]", start, length);
        // Print the separator after each hole (except the last one)
        if (i != (num_holes * 2) - 1){
            dprintf(fd, " - ");
        }
    }
    delete[] reinterpret_cast<uint16_t*>(getListVal);
    return 0;
}

void *MemoryManager::getList()
{
    if(!memoryBlock){
        return nullptr;
    }
    MemoryBlock *currBlock = memoryBlockList;
    int numHoles = 0;
    vector<uint16_t> holes;
    while (currBlock)
    {
        if (currBlock->isAllocated == false)
        {
            numHoles++;
            uint16_t offset = static_cast<uint16_t>(currBlock->startAddress / defaultWordSize);
            uint16_t length = static_cast<uint16_t>(currBlock->sizeInBytes / defaultWordSize);
            holes.push_back(offset);
            holes.push_back(length);
        }
        currBlock = currBlock->next;
    }
    uint16_t *holeArray = new uint16_t[holes.size() + 1];
    holeArray[0] = numHoles;
    for (int i = 1; i <= holes.size(); i++)
    {
        holeArray[i] = holes[i - 1]; // Use i - 1 to access elements in holes
    }
    // save number of holes somewhere
    holesArraySize = holes.size();
    return holeArray;
}

void *MemoryManager::getBitmap()
{
    if(!memoryBlock){
        return nullptr;
    }
    MemoryBlock *currBlock = memoryBlockList;
    int count = 0;
    int index = 2;
    uint8_t *array;
    // total bytes needed
    int total = ((totalMemorySize / defaultWordSize) + 7) / 8;
    uint8_t *bitstream = new uint8_t[total + 2];

    bitstream[0] = total & 0xFF;          // LSB
    bitstream[1] = (total >> 8) & 0xFF;   // MSB

    while (currBlock)
    {
        for (int i = 0; i < currBlock->sizeInBytes / defaultWordSize; i++)
        {
            if (count == 8)
            {
                count = 0;
                index++;
            }
            if (currBlock->isAllocated)
            {
                bitstream[index] |= (1 << (7 - count)); // Set the bit to 1 if it's allocated
            }
            else
            {
                bitstream[index] &= ~(1 << (7 - count)); // Set the bit to 0 if it's free
            }
            count++;
        }
        currBlock = currBlock->next;
    }
    // add extra bytes buffer
    while (count > 0 && count < 8)
    {
        bitstream[index] &= ~(1 << (7 - count)); // Set the remaining bits to 0
        count++;
        if (count == 8)
        {
            count = 0;
            index++;
        }
    }
    // now mirror each byte individually
    for (int i = 2; i < total + 2; i++)
    {
        uint8_t originalByte = bitstream[i];
        uint8_t mirroredByte = 0;

        // Mirror the bits within each byte
        for (int j = 0; j < 8; j++)
        {
            if (originalByte & (1 << j))
            {
                mirroredByte |= (1 << (7 - j));
            }
        }
        bitstream[i] = mirroredByte;
    }
    // print

    return bitstream;
}

int bestFit(int sizeInWords, void *list)
{
    // loop through list[0] amount of times and find smallest possible hole that fits sizeInWords
    uint16_t *memoryList = (uint16_t *)list;
    int index = -1;
    int currBestSize = INT_MAX;
    for (int i = 2; i <= memoryList[0] * 2; i++)
    {
        int blockSize = memoryList[i];
        if (blockSize >= sizeInWords && blockSize < currBestSize)
        {
            currBestSize = blockSize;
            index = memoryList[i - 1];
        }
        i++;
    }
    return index;
}
int worstFit(int sizeInWords, void *list)
{
    uint16_t *memoryList = (uint16_t *)list;
    int index = -1;
    int currWorstSize = -1;
    for (int i = 2; i <= memoryList[0] * 2; i++)
    {
        int blockSize = memoryList[i];
        if (blockSize >= sizeInWords && blockSize > currWorstSize)
        {
            currWorstSize = blockSize;
            index = memoryList[i - 1];
        }
        i++;
    }
    return index;
}
