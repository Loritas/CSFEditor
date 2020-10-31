#include "PALFile.h"

PALFile::PALFile()
{
    memset(_data, 0, kCapacity);
}

PALFile::PALFile(std::string path)
{
	open_from_file(path);
}

bool PALFile::open_from_file(const std::string& path)
{
	std::ifstream fin;
	fin.open(path, std::ios::in | std::ios::binary);
	if (open(fin)) {
		_path = path;
		return true;
	}
	return false;
}

bool PALFile::save_to_file(const std::string& path) const
{
	std::ofstream fout;
	fout.open(path, std::ios::out);
	bool flag = write(fout);
	fout.close();
	return flag;
}

bool PALFile::if_from_file() const
{
	return !_path.empty();
}

std::string PALFile::get_path() const
{
	return _path;
}

PALColor* PALFile::get_array()
{
	return _data;
}

PALColor& PALFile::operator[](size_t index)
{
	if (index <= 255 && index >= 0)
		return _data[index];
	return _data[0];
}

bool PALFile::open(std::ifstream& fin)
{
	if (!fin.is_open())
		return false;
	bool flag = false;
	flag = parse(fin);
	fin.close();
	return flag;
}

bool PALFile::parse(std::ifstream& fin)
{
	fin.read((char*)_data, kCapacity);
	return true;
}

bool PALFile::write(std::ofstream& fout) const
{
	if (!fout.is_open())
		return false;

	fout.write((const char*)_data, kCapacity);
	return true;
}