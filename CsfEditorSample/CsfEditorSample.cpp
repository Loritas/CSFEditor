#include "CSFFile.h"
#include "INIFile.h"

#include <iostream>
#include <locale>
#include <cstdlib>

void init_command_line(int& argc, char** argv)
{
    auto ArgParser = [](char* pArg)
    {
        // printf("%s\n", pArg);
        // system("pause");
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

    // Here is a simple sample for how to use CSFClass
    // Your Tasks are below:
    // 1.merge two csf files
    // 2.tell the differences between two csf file
    // 3(X).what do you need then? :D
    // secsome - 2020/10/29
    CSFFile file("D:\\Project\\CSF Merge\\CSF Merge\\csfs\\stringtable4.csf");
    auto& data = file.get_map();
    data["SBMH"].push_back(std::make_pair(L"MH果然是NT！！！", "This is an extra value"));
    data["SBMH"].push_back(std::make_pair(L"MH果然还是NT！！！", ""));
    file.save_to_file("D:\\stringtable03.csf");
    CSFFile file2("D:\\stringtable03.csf");
    
    INIFile inirules("C:\\Users\\30270\\Desktop\\rulesmo.ini");
    inirules.save_to_file("C:\\Users\\30270\\Desktop\\testrule.ini");

    return 0;
}