#include <iostream>
#include "scanner.hpp"
#include "reader.hpp"
#include "hash.hpp"
#include <fstream>
#include <unordered_set>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/container_hash/hash.hpp>

void controller(std::unordered_set<boost::filesystem::path, boost::hash<boost::filesystem::path>> paths, std::unordered_set<boost::filesystem::path, boost::hash<boost::filesystem::path>> exceptions, std::vector<std::string> masks, size_t size_block ,size_t depth, size_t size_file);