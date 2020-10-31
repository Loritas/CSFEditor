#pragma once

#include "IRAFile.h"

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

class CSFFile final : public IRAFile
{
private:
	using size_type = size_t;
	using key_type = std::string;
	using value_type = std::vector<std::pair<std::wstring, std::string>>;

	// Public functions
public:
	// THE FIVES
	explicit CSFFile() = delete; // Nope

	explicit CSFFile(std::string path);

	~CSFFile() = default;
	CSFFile(CSFFile& lhs) = default;
	CSFFile(CSFFile&& rhs) = default;
	CSFFile& operator=(const CSFFile& rhs) = default;
	CSFFile& operator=(CSFFile&& rhs) = default;

	// Other public functions
	value_type get_value(key_type label);
	value_type& get_value_reference(key_type label);
	
	value_type& operator[](key_type label);

	std::map<key_type, value_type>& get_map();

	bool open_from_file(const std::string& path);
	bool save_to_file(const std::string& path) const;
	bool if_from_file() const;
	std::string get_path() const;

	// In this sample, we use protected for private functions,
	// Though this is a final class, and cannot be inheritted.
protected:
	bool open(std::ifstream& fin);
	bool parse(std::ifstream& fin);
	std::wstring decode(char* src, size_type len);
	void encode(std::wstring& src, char* buffer);
	bool write(std::ofstream& fout) const;

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