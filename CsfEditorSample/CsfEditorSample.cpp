#include "CSFFile.h"

#include <iostream>
#include <locale>
#include <cstdlib>

void init_command_line(int& argc, char** argv)
{
    auto ArgParser = [](char* pArg)
    {
        // TODO : implement
    };

    for (int i = 0; i < argc; ++i)
        ArgParser(argv[i]);
}

void init_locale(std::string localename = "zh_CN.UTF-8")
{
    std::locale lc(localename);
    std::locale::global(lc);
}

void init(int& argc, char** argv)
{
    init_command_line(argc, argv);
    init_locale();
}




int main(int argc, char** argv)
{
    init(argc, argv);
    CSFFile file("D:\\\stringtable02.csf");
    auto& data = file.get_map();
    data["SBMH"].push_back(std::make_pair(L"MH果然是NT！！！", "This is an extra value"));
    data["SBMH"].push_back(std::make_pair(L"MH果然还是NT！！！", ""));
    file.save_to_file("D:\\stringtable03.csf");
    file.~CSFFile();
    CSFFile file2("D:\\stringtable03.csf");
    return 0;
}