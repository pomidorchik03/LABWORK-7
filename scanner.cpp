#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/container_hash/hash.hpp>
#include <unordered_set>
#include "scanner.hpp"
#include "reader.hpp"
#include "mask.hpp"

namespace fs = boost::filesystem;



Scanner::Scanner(std::unordered_set<fs::path, boost::hash<fs::path>> paths, std::unordered_set<fs::path, boost::hash<fs::path>> exceptions, std::vector<std::string> masks, size_t depth, size_t size_file)
{
    if (depth < 0)
    {
        throw "Incorrect depth";
    }
    for (const auto path: paths)
    {
        if (!(fs::exists(path) && fs::is_directory(path)))
        {
            throw "The folder is not in this path, either does not exist, or you specified the wrong folder.";
        }
        if (depth > 0)
        {
            fs::recursive_directory_iterator iter(path, fs::directory_options::skip_permission_denied);
            fs::recursive_directory_iterator end;
            while(iter != end)
            {
                if ((exceptions.find(*iter) != exceptions.end()))
                {
                    ++iter;
                    iter.pop();
                    continue;
                }
                if (iter.depth() > depth)
                {
                    iter.pop();
                    if (iter == end)
                    {
                        break;
                    }
                    continue;
                }

                if (fs::is_regular_file(*iter) && (get_size_file(*iter) > size_file) && is_match(*iter, masks))
                {
                    files_.insert(*iter);
                }
                ++iter;
            }
        }
        else
        {
            for (const auto& file : fs::directory_iterator(path))
            {
                if (!file.is_directory() && (get_size_file(file) > size_file) && is_match(file, masks))
                {
                    files_.insert(file);
                }
            }
        }
    }
}

std::set<fs::path> Scanner::get_files()
{
    return files_;
}

