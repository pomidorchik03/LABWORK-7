#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/container_hash/hash.hpp>


class Scanner
{
private:
    std::set<boost::filesystem::path> files_;
public:
    Scanner(std::unordered_set<boost::filesystem::path, boost::hash<boost::filesystem::path>> paths, std::unordered_set<boost::filesystem::path, boost::hash<boost::filesystem::path>> exceptions, std::vector<std::string> masks, size_t depth, size_t size_file = 1);
    std::set<boost::filesystem::path> get_files();
};