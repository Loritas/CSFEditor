#include "CSFFile.h"

#include <algorithm>

CSFFile::CSFFile(std::string path, bool ordered)
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

bool CSFFile::parse(char* buffer)
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
		if (identifier == 0x4C424C20) // " LBL"
		{
			int numPairs;
			read_int(&numPairs);
			int strLength;
			read_int(&strLength);
			CSFFile::key_type labelstr;
			labelstr.resize(strLength);
			memcpy(&labelstr[0], pos, strLength);
			pos += strLength;
			// CSF labels are not case sensitive.
			std::transform(labelstr.begin(), labelstr.end(), labelstr.begin(), tolower);
			
			CSFFile::value_type value_pair;
			value_pair.resize(numPairs);

			for (int j = 0; j < numPairs; ++j)
			{
				read_int(&identifier);
				read_int(&strLength);
				std::wstring value = decode(pos, strLength);
				std::string exvalue;
				pos += strLength << 1;
				if (identifier == 0x53545257) // "WRTS"
				{
					read_int(&strLength);
					exvalue.resize(strLength);
					memcpy(&exvalue[0], pos, strLength);
				}
				value_pair[j] = std::make_pair(value, exvalue);
			}

			if (!_ordered)
				_udata[labelstr] = value_pair;
			else
				_odata[labelstr] = value_pair;
		}
	}

}

std::wstring CSFFile::decode(char* src, size_t len)
{
	std::wstring ret;
	ret.resize(len << 1);
	char* pret = (char*)&ret[0];
	for (int i = 0; i < len << 1; ++i)
		pret[i] = ~src[i];
	ret.assign((wchar_t*)pret);
	return ret;
}