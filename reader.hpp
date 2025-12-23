#include <boost/filesystem.hpp>
#include <vector>

std::ifstream read(const boost::filesystem::path& path);

size_t get_size_file(const boost::filesystem::path& path);