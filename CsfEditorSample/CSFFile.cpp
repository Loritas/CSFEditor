#include "CSFFile.h"

#include <algorithm>

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
	char* pos = buffer;

	auto read_int = [&pos](const void* dest)
	{
		memcpy((void*)dest, pos, 4);
		pos += 4;
	};

	// Parse CSF Header
	if (memcmp(pos, " FSC", 0x4) != 0)
		return false;
	pos += 4;
	read_int(&_version);
	read_int(&_numLabels);
	read_int(&_numStrings);
	pos += 4; // useless
	read_int(&_lang);

	for (int i = 0; i < _numLabels; ++i)
	{
		int identifier;
		read_int(&identifier);
		pos += 4;
		if (identifier == 0x4C424C20) // " LBL"
		{
			int numPairs;
			read_int(&numPairs);
			int strLength;
			read_int(&strLength);
			std::string labelstr;
			labelstr.resize(strLength);
			memcpy(&labelstr[0], pos, strLength);
			pos += strLength;
			// CSF labels are not case sensitive.
			std::transform(labelstr.begin(), labelstr.end(), labelstr.begin(), tolower);
			read_int(&identifier);
			read_int(&strLength);
			std::wstring value = decode(pos, strLength);

		}
	}

}

std::wstring CSFFile::decode(char* src, size_t len) const
{
	std::wstring ret;
	ret.resize(len << 1);
	char* pret = (char*)&ret[0];
	for (int i = 0; i < len; ++i)
		pret[i] = ~src[i];
	std::wstring ret((wchar_t*)pret);
	return ret;
}