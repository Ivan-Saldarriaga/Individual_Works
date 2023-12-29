#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "../libWad/Wad.h"

using namespace std;

static int getattr_callback(const char *path, struct stat *stbuf)
{
    // memset(stbuf, 0, sizeof(struct stat));
    string pathStr(path);
    Wad *myWad = (Wad *)fuse_get_context()->private_data;
    // Check if path is a directory
    if (myWad->isDirectory(pathStr))
    {
        stbuf->st_mode = S_IFDIR | 0777;
        stbuf->st_nlink = 2;
        return 0;
    }
    else
    {
        if (myWad->isContent(pathStr))
        {
            stbuf->st_mode = S_IFREG | 0777;
            stbuf->st_nlink = 1;
            stbuf->st_size = myWad->getSize(pathStr);
            return 0;
        }
        else
        {
            return -ENOENT;
        }
    }
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    string spath(path);
    Wad *wad = (Wad *)fuse_get_context()->private_data;

    // Check if path corresponds to a valid content file
    if (wad->isContent(spath))
    {
        int val = wad->getContents(spath, buf, size, offset);
        if (val == -1)
        {
            return -errno; // Propagate the correct error code
        }
        return val;
    }

    // File not found
    return -ENOENT;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    vector<string> children;

    int val = ((Wad *)fuse_get_context()->private_data)->getDirectory(path, &children);
    if (val == -1)
    {
        return -ENOENT;
    }
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    for (const string &child : children)
    {
        const char *cString = child.c_str();
        filler(buf, cString, NULL, 0);
    }

    return 0; // Always return success (0) here
}

static int do_mkdir(const char *path, mode_t mode)
{
    Wad *wad = (Wad *)fuse_get_context()->private_data;

    // Check if directory or content with the same name already exists
    if (wad->isDirectory(path) || wad->isContent(path))
    {
        return -EEXIST; // Use EEXIST for "File exists" error
    }

    // Create directory
    wad->createDirectory(path);
    return 0;
}

static int do_mknod(const char *path, mode_t mode, dev_t rdev)
{
    Wad *wad = (Wad *)fuse_get_context()->private_data;

    // Check if directory or content with the same name already exists
    if (wad->isDirectory(path) || wad->isContent(path))
    {
        return -EEXIST; // Use EEXIST for "File exists" error
    }

    // Create regular file
    wad->createFile(path);
    return 0;
}

static int do_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info)
{
    Wad *wad = (Wad *)fuse_get_context()->private_data;

    int val = wad->writeToFile(path, buffer, size, offset);
    if (val == -1)
    {
        return -errno; // Propagate the correct error code
    }

    return val;
}

// https://engineering.facile.it/blog/eng/write-filesystem-fuse/
static struct fuse_operations operations = {
    .getattr = getattr_callback,
    .mknod = do_mknod,
    .mkdir = do_mkdir,
    .read = read_callback,
    .write = do_write,
    .readdir = readdir_callback,
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Error: Not enough arguments." << endl;
        exit(EXIT_FAILURE);
    }

    string wadPath = argv[argc - 2];

    // Relative path!
    if (wadPath.at(0) != '/')
    {
        wadPath = string(get_current_dir_name()) + "/" + wadPath;
    }

    Wad *myWad = Wad::loadWad(wadPath);

    if (myWad == nullptr)
    {
        cerr << "Error: Failed to load the WAD file." << endl;
        exit(EXIT_FAILURE);
    }

    argv[argc - 2] = argv[argc - 1];
    argc--;

    return fuse_main(argc, argv, &operations, myWad);
}
