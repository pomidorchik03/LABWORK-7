#include "controller.hpp"

namespace fs = boost::filesystem;


void controller(std::unordered_set<boost::filesystem::path, boost::hash<boost::filesystem::path>> paths, std::unordered_set<boost::filesystem::path, boost::hash<boost::filesystem::path>> exceptions, std::vector<std::string> masks, size_t size_block, size_t depth, size_t size_file)
{
    try
    {
        std::cout << "Current dir: " << fs::current_path() << "\n";
        Scanner scanner(paths, exceptions, masks, depth, size_file);

        std::vector<std::vector<unsigned int>> hashes;

        Hash hasher(size_block);

        std::vector<unsigned int> hash;

        std::map<std::vector<unsigned int>, std::vector<fs::path>> duplicates;

        for (const auto& file : scanner.get_files())
        {
            hash = hasher.makeCRC32(read(file), get_size_file(file));

            auto it = duplicates.find(hash);
            if (it != duplicates.end())
            {
                it->second.push_back(file);
            }
            else
            {
                duplicates[hash].push_back(file);
            }
            hasher.save(hash);
        }
        for (const auto& [hash, places] : duplicates)
        {   
            if (places.size() > 1)
            {
                for (const auto& place : places)
                {
                    std::cout << place.string() << "\n";
                }
                std::cout << "\n";
            }
        }
    }
    catch(const char* e)
    {
        std::cerr << e << "\n";
    }
}