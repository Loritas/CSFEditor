#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

enum class CSFLanguage
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
	explicit CSFFile(std::string path, bool ordered = false);

	~CSFFile() = default; // No stupid pointers, just uses default
	CSFFile(CSFFile& lhs) = default;
	CSFFile(CSFFile&& rhs) = default;

	// Other public functions
	value_type get_value(key_type label);
	value_type& get_value_reference(key_type label);
	
	value_type& operator[](key_type label);

	std::map<key_type, value_type>& get_map();
	std::unordered_map<key_type, value_type>& get_unordered_map();

	// In this sample, we use protected for private functions,
	// Though this is a final class, and cannot be inheritted.
protected:
	bool open(std::ifstream& fin);
	bool parse(char* buffer);
	
	std::wstring decode(char* src, size_type len);

	static value_type default_value;

	// And for properties, we just use private XD.
private:
	bool _ordered;
	std::string _path;
	std::unordered_map<key_type, value_type> _udata;
	std::map<key_type, value_type> _odata;

	// CSF Header information
	int32_t _version;
	int32_t _numLabels;
	int32_t _numStrings;
	CSFLanguage _lang;

};