#include "reader.hpp"
#include <boost/filesystem.hpp>
#include <vector>
#include <iostream>
#include <fstream>

std::ifstream read(const boost::filesystem::path& path)
{   
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        throw "failed to open file";
    }
        
    
    return file;
}

size_t get_size_file(const boost::filesystem::path& path)
{
    std::ifstream file = read(path);

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    return size;
}