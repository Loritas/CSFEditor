#include "CSFFile.h"

#include <algorithm>

CSFFile::CSFFile(std::string path)
{
	std::ifstream fin;
	fin.open(path, std::ios::in | std::ios::binary);
	if (open(fin)) {
		_path = path;
	}
}

std::map<CSFFile::key_type, CSFFile::value_type>& CSFFile::get_map()
{
	return _data;
}

CSFFile::value_type CSFFile::get_value(key_type label)
{
	auto finditr = _data.find(label);
	if (finditr != _data.end())
		return finditr->second;
	else
		return default_value;
}

CSFFile::value_type& CSFFile::get_value_reference(key_type label)
{
	auto finditr = _data.find(label);
	if (finditr != _data.end())
		return finditr->second;
	else
		return default_value;
}

CSFFile::value_type& CSFFile::operator[](key_type label)
{
	return get_value_reference(label);
}

bool CSFFile::open(std::ifstream& fin)
{
	if (!fin.is_open())
		return false;

	fin.seekg(0, std::ios::end);
	size_type filesize = fin.tellg();
	char* buffer = new char[filesize];
	fin.seekg(0, std::ios::beg);
	fin.read(buffer, filesize);
	fin.close();
	bool flag = parse(buffer);
	delete[] buffer;
	return flag;
}

bool CSFFile::parse(char* buffer)
{
	char* pos = buffer;

	auto read_int = [&pos](const void* dest)
	{
		memcpy((void*)dest, pos, 4);
		pos += 4;
	};

	// Parse CSF header
	if (memcmp(pos, " FSC", 0x4) != 0)
		return false;
	pos += 4;
	read_int(&_version);
	read_int(&_numLabels);
	read_int(&_numStrings);
	pos += 4; // useless
	read_int(&_lang);

	// Read CSF labels
	for (int i = 0; i < _numLabels; ++i)
	{
		// Read CSF label header
		int identifier;
		read_int(&identifier);
		if (identifier == 0x4C424C20) // " LBL"
		{
			int numPairs;
			read_int(&numPairs);
			int strLength;
			read_int(&strLength);
			key_type labelstr;
			labelstr.resize(strLength);
			memcpy(&labelstr[0], pos, strLength);
			pos += strLength;
			// CSF labels are not case sensitive.
			std::transform(labelstr.begin(), labelstr.end(), labelstr.begin(), tolower);
			
			value_type value_pair;
			value_pair.resize(numPairs);

			// Read CSF value pairs
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
				// optimize for memory
				value.shrink_to_fit();
				exvalue.shrink_to_fit();
				
				value_pair[j] = std::make_pair(value, exvalue);
			}
			_data[labelstr] = value_pair;
		}
		else
			return false;
	}
	return true;
}

std::wstring CSFFile::decode(char* src, size_type len)
{
	std::wstring ret;
	ret.resize(len);
	char* pret = (char*)&ret[0];
	for (size_t i = 0; i < len << 1; ++i)
		pret[i] = ~src[i];
	return ret;
}

CSFFile::value_type CSFFile::default_value;