#pragma once

#include <fstream>
#include <string>
#include <map>
#include <vector>

class INIFile final
{
private:
    using INISection = std::map<std::string, std::string>;

public:
    explicit INIFile() = delete;

    explicit INIFile(std::string path);

    ~INIFile() = default;
    INIFile(INIFile& lhs) = default;
    INIFile(INIFile&& rhs) = default;
    INIFile& operator=(const INIFile& rhs) = default;
    INIFile& operator=(INIFile&& rhs) = default;

    bool save_to_file(std::string path) const;
    std::map<std::string, INISection>& get_map();

    INISection& operator[](std::string section);

protected:
    bool open(std::ifstream& fin);
    bool parse(std::ifstream& fin);
    bool write(std::ofstream& fout) const;

private:
    std::string _path;
    std::map<std::string, INISection> _data;
    // std::vector<std::string> _errors;
};

