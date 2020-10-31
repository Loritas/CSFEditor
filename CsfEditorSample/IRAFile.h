#pragma once

#include <fstream>
#include <string>

/// <summary>
/// Interface for RA files
/// </summary>
class IRAFile
{
protected:
    bool virtual open(std::ifstream& fin) = 0;
    bool virtual parse(std::ifstream& fin) = 0;
    bool virtual write(std::ofstream& fout) const = 0;
public:
    bool virtual save_to_file(const std::string& path) const = 0;
    bool virtual open_from_file(const std::string& path) = 0;
    bool virtual if_from_file() const = 0;
    std::string virtual get_path() const = 0;

};