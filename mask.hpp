#include <string>
#include <regex>
#include <boost/filesystem.hpp>

std::regex mask_to_regex(std::string mask);
bool is_match(boost::filesystem::path path, std::vector<std::string> masks);