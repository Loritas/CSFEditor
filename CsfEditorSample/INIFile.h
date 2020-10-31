#pragma once

#include "IRAFile.h"

#include <map> // consider to use unordered_map for efficency if order is not that essential
#include <vector>

class INIFile final : public IRAFile
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

    bool open_from_file(const std::string& path);
    bool save_to_file(const std::string& path) const;
    bool if_from_file() const;
    std::string get_path() const;

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

