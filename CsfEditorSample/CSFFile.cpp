#include "CSFFile.h"

CSFFile::CSFFile(std::string path, bool ordered = false)
{
	std::ifstream fin;
	fin.open(path, std::ios::in | std::ios::binary);
	if (open(fin)) {
		_path = path;
		_ordered = ordered;
	}
}

bool CSFFile::open(std::ifstream& fin)
{
	if (!fin.is_open())
		return false;

	fin.seekg(0, std::ios::end);
	size_t filesize = fin.tellg();
	char* buffer = new char[filesize];
	fin.seekg(0, std::ios::beg);
	fin.read(buffer, filesize);
	fin.close();
	return parse(buffer);
}

bool CSFFile::parse(char* buffer) const
{


}