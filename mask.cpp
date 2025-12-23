#include <string>
#include <regex>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

std::regex mask_to_regex(std::string mask) 
{
    std::string result = "";
    for(char c : mask)
    {
        switch (c)
        {
            case '.': 
                result += "\\.";
                break;
            case '*':
                result += ".*";
                break;
            case '?':
                result += ".";
                break;
            default:
                result += c;
                break;
        }
    }
    std::regex pattern(result);
    return pattern;
 
}

bool is_match(fs::path path, std::vector<std::string> masks)
{
    if (masks.empty())
    {
        return true;
    }

    for (const auto& mask : masks)
    {
        if (std::regex_match(path.filename().string(), mask_to_regex(mask)))
        {
            return true;
        }
    }
    return false;
}
