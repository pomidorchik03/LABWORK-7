#include <iostream>
#include "controller.hpp"
#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <unordered_set>

namespace fs = boost::filesystem;
namespace po = boost::program_options;



int main(int argc, char* argv[]){
    try
    {
        po::options_description desc("asdwasd");
        desc.add_options()
                ("help,h", "Show hint")
                ("path,p", po::value<std::vector<std::string>>()->multitoken()->default_value({}, "") ,"paths scan")
                ("exception,e", po::value<std::vector<std::string>>()->multitoken()->default_value({}, ""), "excluded paths")
                ("mask,m", po::value<std::vector<std::string>>()->multitoken()->default_value({}, ""), "masks")
                ("depth,d", po::value<size_t>()->default_value(0), "depth recursion")
                ("size_block,b", po::value<size_t>()->default_value(1024), "hashing block size")
                ("size_file,f", po::value<size_t>()->default_value(1), "minimal file size")
                ("hash,a", po::value<bool>()->default_value(0), "Hash algorithm: 0 - crc32, 1 - md5");

        po::variables_map vm;

        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << '\n';
            return 0;
        }
        if (vm.count("path"))
        {
            const auto& paths_vec = vm["path"].as<std::vector<std::string>>();

            std::unordered_set<fs::path, boost::hash<fs::path>> paths;
            for (int i = 0; i < paths_vec.size(); i++)
            {
                boost::filesystem::path p(fs::canonical(paths_vec[i]));
                paths.insert(p);
            }

            const auto& exception_vec = vm["exception"].as<std::vector<std::string>>();
            std::unordered_set<fs::path, boost::hash<fs::path>> exceptions;
            for (int i = 0; i < exception_vec.size(); i++)
            {
                boost::filesystem::path e(fs::canonical(exception_vec[i]));
                exceptions.insert(e);
            }

            const auto& masks = vm["mask"].as<std::vector<std::string>>();

            const auto& depth = vm["depth"].as<size_t>();

            const auto& size_block = vm["size_block"].as<size_t>();

            const auto& size_file = vm["size_file"].as<size_t>();

            const auto& hash = vm["hash"].as<bool>();

            controller(paths, exceptions, masks, size_block, depth, size_file);
        }
    }
    catch(const char* e)
    {
        std::cerr << e << "\n";
    }

    std::cin.get();
}