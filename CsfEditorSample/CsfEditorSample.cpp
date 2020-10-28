#include "CSFFile.h"

#include <iostream>
#include <locale>
#include <cstdlib>

int main(int argc, char** argv)
{
    // For Unicode output
    std::locale lc("zh_CN.UTF-8");
    std::locale::global(lc);

    CSFFile file("D:\\stringtable02.csf");
    auto& data = file.get_map();
    size_t cnt = 0;
    for (auto& data_itr : data)
        for (auto& pair_itr : data_itr.second)
        {
            std::cout << "=========================" << std::endl;
            std::cout << ++cnt << ":" << std::endl;
            std::cout << data_itr.first << std::endl;
            std::wcout << pair_itr.first << std::endl;
            std::cout << pair_itr.second << std::endl;
            std::cout << "=========================" << std::endl;
        }
    system("pause");
    return 0;
}