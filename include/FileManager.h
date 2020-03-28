#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <gmp.h>
#include <gmpxx.h>

class FileManager
{
private:

public:
    FileManager();
    FileManager(std::string paths);
    FileManager(std::vector<std::string> paths);
    ~FileManager();
};

#endif