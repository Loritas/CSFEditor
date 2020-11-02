#include "CSFFile.h"

#include <algorithm>

CSFFile::CSFFile(std::string path)
{
	open_from_file(path);
}

std::map<CSFFile::key_type, CSFFile::value_type>& CSFFile::get_map()
{
	return _data;
}

CSFFile::value_type CSFFile::get_value(key_type label)
{
	return _data[label];
}

CSFFile::value_type& CSFFile::get_value_reference(key_type label)
{
	return _data[label];
}

bool CSFFile::open_from_file(const std::string& path)
{
	std::ifstream fin;
	fin.open(path, std::ios::in | std::ios::binary);
	if (open(fin)) {
		_path = path;
		return true;
	}
	return false;
}

bool CSFFile::if_from_file() const
{
	return !_path.empty();
}

std::string CSFFile::get_path() const
{
	return _path;
}

bool CSFFile::save_to_file(const std::string& path) const
{
	std::ofstream fout;
	fout.open(path, std::ios::out | std::ios::binary);
	bool flag = false;
	if (fout.is_open())
		flag = write(fout);
	fout.close();
	return flag;
}

CSFFile::value_type& CSFFile::operator[](key_type label)
{
	return get_value_reference(label);
}

bool CSFFile::open(std::ifstream& fin)
{
	if (!fin.is_open())
		return false;
	bool flag = false;
	flag = parse(fin);
	fin.close();
	return flag;
}

bool CSFFile::parse(std::ifstream& fin)
{
	fin.seekg(0, std::ios::end);
	size_type filesize = fin.tellg();
	char* buffer = new char[filesize];
	fin.seekg(0, std::ios::beg);
	fin.read(buffer, filesize);

	char* pos = buffer;

	auto read_int = [&pos](const void* dest)
	{
		memcpy((void*)dest, pos, 4);
		pos += 4;
	};

	// Parse CSF header
	if (memcmp(pos, " FSC", 0x4) != 0) {
		delete[] buffer;
		return false;
	}
	pos += 4;
	read_int(&_version);
	int _numLabels;
	read_int(&_numLabels);
	pos += 4; // numstrings
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
					pos += strLength;
				}
				// optimize for memory
				value.shrink_to_fit();
				exvalue.shrink_to_fit();
				
				value_pair[j] = std::make_pair(value, exvalue);
			}
			_data[labelstr] = value_pair;
		}
		else {
			delete[] buffer;
			return false;
		}
	}
	delete[] buffer;
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

void CSFFile::encode(const std::wstring& src, char* buffer) const
{
	size_t length = src.length() << 1;
	char* pSrc = (char*)&src[0];
	for (size_t i = 0; i < length; ++i)
		buffer[i] = ~pSrc[i];
}

bool CSFFile::write(std::ofstream& fout) const
{
	if (!fout.is_open())
		return false;

	auto write_to_stream = [&fout](const void* buffer, size_t size = 4) {
		fout.write((const char*)buffer, size);
	};

	auto write_int = [&fout](int n){
		fout.write((const char*)&n, 4);
	};

	// CSF header
	write_to_stream(" FSC");
	write_int(_version);
	write_int(_data.size());
	int _stringNums = 0;
	for (auto& x : _data)
		_stringNums += x.second.size();
	write_int(_stringNums);
	write_to_stream("SBMH"); // useless
	write_int((int)_lang);

	// CSF labels
	for (auto& lbl : _data)
	{
		// label
		write_to_stream(" LBL");
		write_int(lbl.second.size());
		write_int(lbl.first.length());
		write_to_stream(lbl.first.c_str(), lbl.first.length());

		// value
		for (auto& pr : lbl.second)
		{
			if (pr.second.empty())
				write_to_stream(" RTS");
			else
				write_to_stream("WRTS");

			write_int(pr.first.length());
			char* buffer = new char[pr.first.length() << 1];
			encode(pr.first, buffer);
			write_to_stream(buffer, pr.first.length() << 1);
			delete[] buffer;

			if (!pr.second.empty())
			{
				write_int(pr.second.length());
				write_to_stream(pr.second.c_str(), pr.second.length());
			}
		}
	}
	return true;
}

CSFFile::value_type CSFFile::default_value;