#include "hash.hpp"
#include <boost/filesystem.hpp>
#include <openssl/md5.h>
#include <boost/crc.hpp>
#include <vector>
#include <iostream>
#include <fstream>

Hash::Hash(size_t block_size) : block_size_(block_size){}

unsigned int MD5(const char* block, const size_t block_size)
{
    const unsigned int NEEDS_BYTES = 16;
    unsigned char data[NEEDS_BYTES];
    MD5(reinterpret_cast<const unsigned char*>(block), block_size, data);

    unsigned int hash;
    std::memcpy(&hash, data, sizeof(unsigned int));
    return hash;
}

unsigned int CRC32(const char* block, const size_t block_size)
{
    boost::crc_32_type crc;
    
    crc.process_bytes(block, block_size);
    return crc.checksum();
}
 
std::vector<unsigned int> Hash::makeMD5(std::ifstream file, size_t size_file)
{
    std::vector<unsigned int> hash;
    
    std::vector<char> block(block_size_); 

    file.clear();
    file.seekg(0, std::ios::beg);

    while (file.read(block.data(), block_size_) || file.gcount() > 0) {
        std::streamsize bytes_read = file.gcount();

        if (bytes_read < static_cast<std::streamsize>(block_size_)) {
            std::fill(block.begin() + bytes_read, block.end(), '\0');
        }

        hash.push_back(MD5(block.data(), block_size_));

        if (file.eof()) 
        {
            break;
        }
    }

    return hash;
}

std::vector<unsigned int> Hash::makeCRC32(std::ifstream file, size_t size_file)
{
    std::vector<unsigned int> hash;
    
    std::vector<char> block(block_size_); 

    file.clear();
    file.seekg(0, std::ios::beg);

    while (file.read(block.data(), block_size_) || file.gcount() > 0) {
        std::streamsize bytes_read = file.gcount();

        if (bytes_read < static_cast<std::streamsize>(block_size_)) {
            std::fill(block.begin() + bytes_read, block.end(), '\0');
        }

        hash.push_back(CRC32(block.data(), block_size_));

        if (file.eof()) 
        {
            break;
        }
    }

    return hash;
}

void Hash::save(const std::vector<unsigned int> hash)
{
    caсhe_.push_back(hash);
}
