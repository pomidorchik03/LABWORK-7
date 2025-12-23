#include <boost/filesystem.hpp>
#include <vector>

class Hash
{
private:
    std::vector<std::vector<unsigned int>> caсhe_;
    const size_t block_size_;
public:
    Hash(size_t block_size);
    
    std::vector<unsigned int> makeMD5(std::ifstream file, size_t size_file);
    std::vector<unsigned int> makeCRC32(std::ifstream file, size_t size_file);
    void save(const std::vector<unsigned int> hash);
    bool find(const std::vector<unsigned int> hash) const;
};