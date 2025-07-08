#pragma once
#include <string>

class FileReader {
	public:
	explicit FileReader(std::string  filename);
	[[nodiscard]] std::string read_all() const;

	private:
	std::string filename;
};
