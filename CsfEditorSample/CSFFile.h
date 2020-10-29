#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>

enum class CSFLanguage : int
{
	US = 0,
	UK,
	German,
	French,
	Spanish,
	Italian,
	Japanese,
	Jabberwockie, // EasterEgg? XD
	Korean,
	Chinese,
	AresUnknown = -1 // Ares uses this
};

class CSFFile final
{
private:
	using size_type = size_t;
	using key_type = std::string;
	using value_type = std::vector<std::pair<std::wstring, std::string>>;

	// Public functions
public:
	// THE FIVES
	explicit CSFFile() = delete; // Nope

	// We use unordered_map defaultly for effeciency.
	explicit CSFFile(std::string path);

	~CSFFile() = default; // No stupid pointers, just uses default
	CSFFile(CSFFile& lhs) = default;
	CSFFile(CSFFile&& rhs) = default;

	// Other public functions
	value_type get_value(key_type label);
	value_type& get_value_reference(key_type label);
	
	value_type& operator[](key_type label);

	std::map<key_type, value_type>& get_map();

	bool save_to_file(std::string path);

	// In this sample, we use protected for private functions,
	// Though this is a final class, and cannot be inheritted.
protected:
	bool open(std::ifstream& fin);
	bool parse(char* buffer);
	std::wstring decode(char* src, size_type len);

	bool write(std::ofstream& fout);

private:
	static value_type default_value;

	// And for properties, we just use private XD.
private:
	std::string _path;
	std::map<key_type, value_type> _data;

	// CSF Header information
	int32_t _version;
	CSFLanguage _lang;

};