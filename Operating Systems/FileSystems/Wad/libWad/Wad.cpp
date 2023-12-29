#include "Wad.h"

Wad *Wad::loadWad(const string &path)
{
    // lead the header only, populating the varibales for filetype, # descriptors and descriptor offset!
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        // cout << "Error Opening File... Womp womp wommmmmmmp </3" << endl;
        return nullptr;
    }
    // cout << "File opened succesfully... WOOT WOOT <3" << endl;
    //  now load in the header information! 12 bytes
    char headerData[12];
    file.read(headerData, sizeof(headerData));
    // edgecase?
    // continue
    // create new WAD
    Wad *wad = new Wad();
    wad->filePath = path;
    wad->magic.assign(headerData, 4);
    // cout << "Magic Type: " << wad->magic << endl;
    wad->numDescriptors = *reinterpret_cast<int *>(headerData + 4);
    // cout << "# of Descriptors: " << wad->numDescriptors << endl;
    wad->descriptorOffset = *reinterpret_cast<int *>(headerData + 8);
    wad->descOff = wad->descriptorOffset;
    // cout << "Descriptor Offset: " << wad->descriptorOffset << endl;
    //   load in '/' directory
    //   initialize rootDirectory
    wad->rootDirectory = new Descriptor;
    wad->rootDirectory->name = "/";
    wad->rootDirectory->filePath = "/";
    wad->rootDirectory->isDirectory = true;
    // ADD TO MAP
    wad->pathMap[wad->rootDirectory->filePath] = wad->rootDirectory;
    // start loading in file data at offset!
    /*******************/
    file.seekg(wad->descriptorOffset, ios::beg);
    // directory patterns
    regex pattern1("^(E\\dM\\d)$");
    regex pattern2("^(\\w{1,2})(_START)$");
    regex pattern3("^(\\w{1,2})(_END)$");
    deque<Descriptor *> directories;
    directories.push_back(wad->rootDirectory);
    for (int i = 0; i < wad->numDescriptors; i++)
    {
        char descriptorData[16];
        file.read(descriptorData, sizeof(descriptorData));
        Descriptor *newDescriptor = new Descriptor;
        newDescriptor->offset = *reinterpret_cast<int *>(descriptorData);
        newDescriptor->length = *reinterpret_cast<int *>(descriptorData + 4);
        newDescriptor->name.assign(descriptorData + 8, 8);
        // remove null stuff
        newDescriptor->name.erase(remove(newDescriptor->name.begin(), newDescriptor->name.end(), '\0'), newDescriptor->name.end());
        // if E#M#
        smatch matches;
        // cout << "Looking to add: " << newDescriptor->name << endl;
        if (regex_match(newDescriptor->name, pattern1))
        {
            // add to parent vector
            newDescriptor->isDirectory = true;
            directories.back()->children.push_back(newDescriptor);
            directories.push_back(newDescriptor);
            // now add element into map
            string prePath = wad->pathMaker(directories);
            // pathMap.insert
            wad->pathMap.insert({prePath, newDescriptor});
            // cout << prePath << endl;
            for (int j = 0; j < 10; j++)
            {
                char descriptorData[16];
                file.read(descriptorData, sizeof(descriptorData));
                Descriptor *newFile = new Descriptor;
                newFile->offset = *reinterpret_cast<int *>(descriptorData);
                newFile->length = *reinterpret_cast<int *>(descriptorData + 4);
                newFile->name.assign(descriptorData + 8, 8);
                newFile->isDirectory = false;
                // remove null stuff
                newFile->name.erase(remove(newFile->name.begin(), newFile->name.end(), '\0'), newFile->name.end());
                // cout << "Looking to add: " << newFile->name << endl;
                directories.back()->children.push_back(newFile);
                directories.push_back(newFile);
                // pathMap.insert
                string prePath = wad->pathMaker(directories);
                wad->pathMap.insert({prePath, newFile});
                // cout << prePath << endl;
                directories.pop_back();
            }
            i += 10;
            directories.pop_back();
        }
        else if (regex_match(newDescriptor->name, matches, pattern2))
        { //_START
          // add to parent vector
            newDescriptor->isDirectory = true;
            newDescriptor->name = matches[1];
            directories.back()->children.push_back(newDescriptor);
            directories.push_back(newDescriptor);
            string prePath = wad->pathMaker(directories);
            // pathMap.insert
            wad->pathMap.insert({prePath, newDescriptor});
            // cout << prePath << endl;
        }
        else if (regex_match(newDescriptor->name, matches, pattern3))
        { //_END
            directories.pop_back();
        }
        else
        { // its a file
          // add to parent vector
            newDescriptor->isDirectory = false;
            directories.back()->children.push_back(newDescriptor);
            directories.push_back(newDescriptor);
            // pathMap.insert
            string prePath = wad->pathMaker(directories);
            wad->pathMap.insert({prePath, newDescriptor});
            // cout << prePath << endl;
            directories.pop_back();
        }
    }
    file.close();
    return wad;
}
string Wad::pathMaker(deque<Descriptor *> paths)
{
    string totalPath;
    int size = paths.size();
    for (int i = 0; i < size - 1; i++)
    {
        if (i == 0)
        {
            totalPath.append(paths.front()->name);
        }
        else
        {
            totalPath.append(paths.front()->name);
            totalPath.append("/");
        }
        paths.pop_front();
    }
    totalPath.append(paths.front()->name);
    return totalPath;
}
string Wad::getMagic()
{
    return magic;
}

bool Wad::isContent(const string &path)
{
    for (const auto &entry : pathMap)
    {
        if (entry.first == path && entry.second->isDirectory == false)
        {
            return true;
        }
    }
    return false;
}

bool Wad::isDirectory(const string &path)
{
    string cleanPath = path;
    if (cleanPath != "/")
    {
        cleanPath = (path.back() == '/') ? path.substr(0, path.length() - 1) : path;
    }
    // cout << "looking for: " << cleanPath << endl;
    for (const auto &entry : pathMap)
    {
        // cout << entry.first << endl;
        if (entry.first.compare(cleanPath) == 0)
        {
            // cout << "found: " << entry.first << endl;
        }
        if (entry.first.compare(cleanPath) == 0 && entry.second->isDirectory == true)
        {
            // cout << "FOUND Directory" << endl;
            return true;
        }
    }
    return false;
}

int Wad::getSize(const string &path)
{
    for (const auto &entry : pathMap)
    {
        if (entry.first.compare(path) == 0 && entry.second->isDirectory == false)
        {
            return entry.second->length;
        }
    }
    return -1;
}
int Wad::getDirectory(const string &path, vector<string> *directory)
{
    string cleanPath = path;
    if (cleanPath != "/")
    {
        cleanPath = (path.back() == '/') ? path.substr(0, path.length() - 1) : path;
    }
    for (const auto &entry : pathMap)
    {
        if (entry.first.compare(cleanPath) == 0 && entry.second->isDirectory == true)
        {
            // add into directory
            for (int i = 0; i < entry.second->children.size(); i++)
            {
                directory->push_back(entry.second->children[i]->name);
            }
            return entry.second->children.size();
        }
    }
    return -1;
}

int Wad::getContents(const string &path, char *buffer, int length, int offset)
{
    for (const auto &entry : pathMap)
    {
        if (entry.first.compare(path) == 0 && entry.second->isDirectory == false)
        {
            ifstream file(filePath, ifstream::binary);
            if (offset >= entry.second->length)
            {
                return 0;
            }
            else
            {
                file.seekg(entry.second->offset + offset, ios::beg);
                if (entry.second->length > (length + offset))
                {
                    file.read(buffer, length);
                    buffer[length] = '\0';
                    file.close();
                    return length;
                }
                else if (entry.second->length == (length + offset))
                {
                    file.read(buffer, entry.second->length);
                    buffer[entry.second->length] = '\0';
                    file.close();
                    return length;
                }
                else
                {
                    file.read(buffer, entry.second->length - offset);
                    buffer[entry.second->length - offset] = '\0';
                    file.close();
                    return entry.second->length - offset;
                }
            }
        }
    }
    return -1;
}
void Wad::createDirectory(const string &path)
{
    // parse path!
    // find last '/'
    string cleanPath = path;
    string existingPath = "";
    string parentDirectory = "";
    string newDirectory = "";
    regex pattern1("^/\\w+");
    regex pattern2("^/\\w+/");
    if (regex_match(path, pattern1))
    {
        // {/newDir}
        parentDirectory = "/";
        existingPath = "/";
        size_t lastSlash = cleanPath.find_last_of('/');
        newDirectory = cleanPath.substr(lastSlash + 1);
        // cout << "CASE 1: " << parentDirectory << ":: " << newDirectory << endl;
    }
    else if (regex_match(path, pattern2))
    {
        // {/newDir/}
        parentDirectory = "/";
        existingPath = "/";
        cleanPath = (path.back() == '/') ? path.substr(0, path.length() - 1) : path;
        size_t lastSlash = cleanPath.find_last_of('/');
        newDirectory = cleanPath.substr(lastSlash + 1);
        // cout << "CASE 2: " << parentDirectory << ":: " << newDirectory << endl;
    }
    else
    {
        cleanPath = (path.back() == '/') ? path.substr(0, path.length() - 1) : path;
        size_t lastSlash = cleanPath.find_last_of('/');
        existingPath = cleanPath.substr(0, lastSlash);
        newDirectory = cleanPath.substr(lastSlash + 1);
        lastSlash = existingPath.find_last_of('/');
        parentDirectory = existingPath.substr(lastSlash + 1);
        // cout << "CASE 3: " << parentDirectory << ":: " << newDirectory << endl;
    }
    regex patternEM("^(E\\dM\\d)$");
    regex patternWORD("^(\\w{1,2})$");
    // TODO: all edge cases that would result in failure
    if (isDirectory(cleanPath))
    {
        // cout << "Directory already exists" << endl;
        return;
    }
    else if (regex_match(parentDirectory, patternEM))
    {
        // cout << "Directory cannot be E#M#" << endl;
        return;
    }
    else if (!regex_match(newDirectory, patternWORD))
    {
        // cout << "Invalid input" << endl;
        return;
    }
    else if (!isDirectory(existingPath))
    {
        // cout << "Parent is not a directory" << endl;
        return;
    }
    Descriptor *newDescriptor = new Descriptor;
    cleanPath.erase(std::remove(cleanPath.begin(), cleanPath.end(), '\0'), cleanPath.end());

    newDescriptor->filePath = cleanPath;
    newDescriptor->isDirectory = true;
    newDescriptor->length = 0;
    newDescriptor->offset = 0;
    newDescriptor->name = newDirectory;
    // push back child & add path to map
    for (const auto &entry : pathMap)
    {
        if (entry.first.compare(existingPath) == 0 && entry.second->isDirectory == true)
        {
            pathMap.insert({newDescriptor->filePath, newDescriptor});
            entry.second->children.push_back(newDescriptor);
            // cout << "Successfully added new path: " << newDescriptor->filePath << endl;
            fstream file(filePath, ios::binary | ios::in | ios::out);
            file.seekp(descriptorOffset, ios::beg);
            dfsDirectory(rootDirectory, file);
            // now change header # of descriptors + 2
            numDescriptors += 2;
            file.seekp(sizeof(int), std::ios::beg);
            file.write(reinterpret_cast<const char *>(&numDescriptors), sizeof(numDescriptors));
            file.close();
            return;
        }
    }
}
void Wad::dfsDirectory(Descriptor *root, fstream &file)
{
    regex patternEM("^(E\\dM\\d)$");
    if (root)
    {
        int offset = root->offset;
        int length = root->length;
        string name = root->name;
        if (root->name != "/")
        {
            if (root->isDirectory && !regex_match(root->name, patternEM))
            {
                string newName = name + "_START";
                // write to wad
                file.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
                file.write(reinterpret_cast<const char *>(&length), sizeof(length));
                // pad with null characters if small!
                newName.resize(8, '\0');
                file.write(newName.c_str(), 8);

                // cout << root->name << "_START ";
            }
            else
            {
                file.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
                file.write(reinterpret_cast<const char *>(&length), sizeof(length));
                // pad with null characters if small!
                name.resize(8, '\0');
                file.write(name.c_str(), 8);
                // cout << root->name << " ";
            }
        }
        for (Descriptor *child : root->children)
        {
            dfsDirectory(child, file);
        }
        if (root->isDirectory && !regex_match(root->name, patternEM) && root->name != "/")
        {
            string newName = name + "_END";
            file.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
            file.write(reinterpret_cast<const char *>(&length), sizeof(length));
            // pad with null characters if string is less than 8! (necessary for rereading .wad)
            newName.resize(8, '\0');
            file.write(newName.c_str(), 8);
            // cout << root->name << "_END ";
        }
    }
}
void Wad::createFile(const string &path)
{
    // parse path!
    // find last '/'
    string cleanPath = path;
    string existingPath = "";
    string parentDirectory = "";
    string newDirectory = "";
    regex pattern1("^/[\\w.]+");
    // regex pattern2("^/\\w.\\w+/");
    // cout << cleanPath << endl;
    // TODO: check if path ends in / then you cant add it?
    if (regex_match(path, pattern1))
    {
        // {/newDir}
        parentDirectory = "/";
        existingPath = "/";
        size_t lastSlash = cleanPath.find_last_of('/');
        newDirectory = cleanPath.substr(lastSlash + 1);
        // cout << "CASE 1: " << parentDirectory << ":: " << newDirectory << endl;
    }
    else
    {
        cleanPath = (path.back() == '/') ? path.substr(0, path.length() - 1) : path;
        size_t lastSlash = cleanPath.find_last_of('/');
        existingPath = cleanPath.substr(0, lastSlash);
        newDirectory = cleanPath.substr(lastSlash + 1);
        lastSlash = existingPath.find_last_of('/');
        parentDirectory = existingPath.substr(lastSlash + 1);
        // cout << "CASE 3: " << parentDirectory << ":: " << newDirectory << endl;
    }
    // cout << "EXISITNIG PATH: " << existingPath << endl;
    regex patternEM("^(E\\dM\\d)$");
    regex patternSTART("^(\\w{1,2})(_START)$");
    regex patternEND("^(\\w{1,2})(_END)$");
    // cout << parentDirectory << endl;
    if (isContent(cleanPath))
    {
        // cout << "File with this path already exists." << endl;
        return;
    }
    else if (regex_match(parentDirectory, patternEM))
    {
        // cout << "File cannot be added to E#M# directory." << endl;
        return;
    }
    else if (!isDirectory(existingPath))
    {
        // cout << existingPath << "is not existing directory." << endl;
        return;
    }
    else if (regex_match(newDirectory, patternEM) || regex_match(newDirectory, patternSTART) || regex_match(newDirectory, patternEND))
    {
        // cout << "Illegal name for the file!" << endl;
        return;
    }
    else if (newDirectory.size() > 8)
    {
        // cout << "New file name is too large!" << endl;
        return;
    }
    Descriptor *newDescriptor = new Descriptor;
    cleanPath.erase(std::remove(cleanPath.begin(), cleanPath.end(), '\0'), cleanPath.end());

    newDescriptor->filePath = cleanPath;
    newDescriptor->isDirectory = false;
    newDescriptor->length = 0;
    newDescriptor->offset = 0;
    newDescriptor->name = newDirectory;
    for (const auto &entry : pathMap)
    {
        if (entry.first.compare(existingPath) == 0 && entry.second->isDirectory == true)
        {
            pathMap.insert({newDescriptor->filePath, newDescriptor});
            entry.second->children.push_back(newDescriptor);
            // cout << "Successfully added new path: " << newDescriptor->filePath << endl;
            fstream file(filePath, ios::binary | ios::in | ios::out);
            file.seekp(descriptorOffset, ios::beg);
            dfsDirectory(rootDirectory, file);
            // now change header # of descriptors + 2
            numDescriptors += 1;
            file.seekp(sizeof(int), std::ios::beg);
            file.write(reinterpret_cast<const char *>(&numDescriptors), sizeof(numDescriptors));
            file.close();
            return;
        }
    }
}

int Wad::writeToFile(const string &path, const char *buffer, int length, int offset)
{
    for (const auto &entry : pathMap)
    {
        if (entry.first == path && entry.second->isDirectory == false && entry.second->length != 0)
        {
            // cout << "File not empty!" << endl;
            return 0;
        }
        if (entry.first == path && entry.second->isDirectory == true)
        {
            // cout << "File is directory, not file!" << endl;
            return -1;
        }
        if (entry.first == path && entry.second->isDirectory == false && entry.second->length == 0)
        {
            // i need to find the start of descriptor list and move it length bytes!
            entry.second->offset = descriptorOffset;
            entry.second->length = length;
            fstream file(filePath, ios::binary | ios::in | ios::out);
            file.seekp(descriptorOffset, ios::beg);
            // print contents
            file.write(buffer, length);
            file.seekp(descriptorOffset + length, ios::beg);
            dfsDirectory(rootDirectory, file);
            descriptorOffset = descriptorOffset + length;
            file.seekp(8, std::ios::beg);
            file.write(reinterpret_cast<const char *>(&descriptorOffset), sizeof(descriptorOffset));
            file.close();
            return length;
        }
    }
    return -1;
}
// MAIN
// HOW TO RUN:
//  g++ -o output Wad.cpp && ./output