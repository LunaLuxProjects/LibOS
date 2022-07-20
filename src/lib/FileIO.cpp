#include "Cmake.h"
#include "IFileIO.hpp"
#include <libos/FileIO.h>
#include <lstd/StringLexer.h>
#include <vector>
#if CMAKE_SYSTEM_NUMBER == 0
#    include "Linux/Linux.hpp"
#endif
#if CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
#    include "Windows/Windows.hpp"
#endif
lstd::string asset_path = "NOT_SET";

losResult losSetAssetPath(const char *path)
{
    asset_path = path;
    return LOS_SUCCESS;
}

std::vector<lstd::string> split(lstd::string s) noexcept
{
    std::vector<lstd::string> tokens;
    lstd::string token;
    lstd::Lexer lexer(s);
    while (lexer.eof())
    {
        lstd::Lexer::Lex lex = lexer.getLexWord();
        if (lexer.getSubString(lex.start, lex.end) == '/' || lexer.getSubString(lex.start, lex.end) == '\\')
        {
            //FIXME: lstd this should not move the token but copy idk why it's not
            tokens.emplace_back(*new lstd::string(token.c_str()));
            token.clear();
        }
        else
            token += lexer.getSubRealString(lex.start, lex.end);
    }
    //FIXME: lstd this should not move the token but copy idk why it's not
    tokens.emplace_back(*new lstd::string(token.c_str()));
    token.clear();
    return tokens;
}

lstd::string getCorrectPath(const char *path)
{
    lstd::string ret_path;
    auto path_tokens = split(std::move(path));
    for (auto &tokens : path_tokens)
    {
        if (tokens[0] == '$')
        {
            lstd::string command;
            {
                lstd::string processed_1 = tokens.substr(2);
                command = processed_1.substr(0, processed_1.size() - 1);
            }
            if (command == "binary_base")
            {
                auto sun_tuk = split(std::move(getCurrentPath()));
#if CMAKE_SYSTEM_NUMBER == 0
                for (data_size i = 0; i < sun_tuk.size(); i++)
#endif
#if CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
                    for (data_size i = 0; i < sun_tuk.size() - 1; i++)
#endif
                        ret_path += (sun_tuk[i] += '/');
            }
            else if (command == "asset_base")
            {
                ret_path += asset_path;
                ret_path += '/';
            }
        }
        else if(tokens.find('.') != lstd::string::npos)
            ret_path += tokens;
        else
            ret_path += (tokens += '/');
    }
    return ret_path;
}
