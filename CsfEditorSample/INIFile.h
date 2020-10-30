#pragma once

#include <fstream>
#include <string>
#include <map>
#include <vector>

class INIFile final
{
private:
    using INISection = std::map<std::string, std::string>;
    // using OrderedINISection = std::map<unsigned int, INIPair>;

public:
    explicit INIFile() = delete;

    explicit INIFile(std::string path);

    ~INIFile() = default;
    INIFile(INIFile& lhs) = default;
    INIFile(INIFile&& rhs) = default;
    INIFile& operator=(const INIFile& rhs) = default;
    INIFile& operator=(INIFile&& rhs) = default;

protected:
    bool open(std::ifstream& fin);
    bool parse(std::ifstream& fin);
    bool write(std::ofstream& fout);

private:
    std::string _path;
    std::map<std::string, INISection> _data;
    std::vector<std::string> _errors;
};

