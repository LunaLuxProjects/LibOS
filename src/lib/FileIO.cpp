#include "Cmake.h"
#include "IFileIO.hpp"
#include <Components/FileIO.h>
#include <sstream>
#include <string>
#include <vector>

#if CMAKE_SYSTEM_NUMBER == 0
#    include "Linux/Linux.hpp"
#endif
#if CMAKE_SYSTEM_NUMBER == 1
#    include "Windows/Windows.hpp"
#endif
std::string asset_path = "NOT_SET";

losResult losSetAssetPath(const char *path)
{
    asset_path = path;
    return LOS_SUCCESS;
}

std::vector<std::string> split(std::string s, const char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        if (token.find('\\') != std::string::npos)
        {
            auto path_tokens = split(token.c_str(), '\\');
            for (auto t : path_tokens)
                tokens.push_back(t);
        }
        else
            tokens.push_back(token);
    }
    return tokens;
}

std::string getCorrectPath(const char *path)
{
    std::string ret_path;
    auto path_tokens = split(std::move(path), '/');
    for (auto &tokens : path_tokens)
    {
        if (tokens.starts_with('$'))
        {
            std::string command;
            {
                std::string processed_1 = tokens.substr(2);
                command = processed_1.substr(0, processed_1.size() - 1);
            }
            if (command == "binary_base")
            {
                std::string path_os;
                path_os = std::move(getCurrentPath());
                auto sun_tuk = split(std::move(path_os), '/');
#if CMAKE_SYSTEM_NUMBER == 0
                for (size i = 0; i < sun_tuk.size(); i++)
#endif
#if CMAKE_SYSTEM_NUMBER == 1
                for (size i = 0; i < sun_tuk.size() - 1; i++)
#endif
                    ret_path += (sun_tuk[i] += "/");
            }
            else if (command == "asset_base")
            {
                ret_path += asset_path;
                ret_path += '/';
            }
        }
        else
            ret_path += (tokens += "/");
    }
    return ret_path.substr(0, ret_path.size() - 1);
}
