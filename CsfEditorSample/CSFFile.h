#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

class CSFFile final
{
	// Public functions
public:
	// THE FIVES
	CSFFile() = delete; // Nope

	// We use unordered_map defaultly for effeciency.
	explicit CSFFile(std::string path, bool ordered = false);

	~CSFFile() = default; // No stupid pointers, just uses default
	CSFFile(CSFFile& lhs) = default;
	CSFFile(CSFFile&& rhs) = default;


	// Other public functions



	// In this sample, we use protected for private functions,
	// Though this is a final class, and cannot be inheritted.
protected:
	bool open(std::ifstream& fin);
	bool parse(char* buffer) const;
	
	// And for properties, we just use private XD.
private:
	using key_type = std::string;
	using value_type = std::vector<std::string>;

	bool _ordered;
	std::string _path;
	std::unordered_map<key_type, value_type> _udata;
	std::map<key_type, value_type> _odata;

};

