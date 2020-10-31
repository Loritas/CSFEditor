#pragma once

#include "IRAFile.h"

struct PALColor {
    unsigned char R, G, B;
};

class PALFile final : public IRAFile
{
private:
    const int kCapacity = 768;

public:
    explicit PALFile();
    explicit PALFile(std::string path);

    ~PALFile() = default;
    PALFile(PALFile& lhs) = default;
    PALFile(PALFile&& rhs) = default;
    PALFile& operator=(const PALFile& rhs) = default;
    PALFile& operator=(PALFile&& rhs) = default;

    bool open_from_file(const std::string& path);
    bool save_to_file(const std::string& path) const;
    bool if_from_file() const;
    std::string get_path() const;

    PALColor* get_array();

    PALColor& operator[](size_t index);

protected:
    bool open(std::ifstream& fin);
    bool parse(std::ifstream &fin);
    bool write(std::ofstream& fout) const;

private:
    PALColor _data[256];
    std::string _path;
};

