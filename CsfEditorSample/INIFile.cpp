#include "INIFile.h"

INIFile::INIFile(std::string path)
{
	std::ifstream fin;
	fin.open(path, std::ios::in);
	if (open(fin)) {
		_path = path;
	}
}

bool INIFile::open(std::ifstream& fin)
{
	if (!fin.is_open())
		return false;
	bool flag = false;
	flag = parse(fin);
	return flag;
}

bool INIFile::parse(std::ifstream& fin)
{
	auto trim_string = [](std::string& str)
	{
		size_t commit_flag = str.find(';');
		if (commit_flag != str.npos)
			str = str.substr(0, commit_flag);
		str = str.substr
		(
			str.find_first_not_of(' '),
			str.find_last_not_of(' ')
		);
	};

	enum parseing_stage {
		finding_section,
		parsing_section,
		parsing_exit
	};

	parseing_stage parsing_state = finding_section;
	std::string section_name;

	std::string line_buffer;
	while (!fin.eof())
	{
		if (parsing_state != parsing_exit)
		{
			std::getline(fin, line_buffer);
			trim_string(line_buffer);
		}
		
		if (parsing_state == finding_section)
		{
			if (line_buffer[0] == '[')
			{
				size_t back_bracket = line_buffer.find_first_of(']');
				if (back_bracket == line_buffer.npos || back_bracket == 1)
					continue;
				section_name = line_buffer.substr(1, back_bracket);
				parsing_state = parsing_section;
				continue;
			}
		}
		else
		{
			if (line_buffer.find('[') != line_buffer.npos)
			{
				parsing_state = parsing_exit;
				continue;
			}

			size_t equal_flag = line_buffer.find_first_of('=');
			if (equal_flag == line_buffer.npos)
				continue;
			std::string pair_key = line_buffer.substr(0, equal_flag);
			std::string pair_value = line_buffer.substr(equal_flag + 1);
			_data[section_name][pair_key] = pair_value;
		}
	}

	return true;
}