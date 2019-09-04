#include "FileSystem.hpp"
#include <fstream>

namespace yare
{

void FileSystem::readFile(const std::string& filename, std::string& contents) {
	std::ifstream file(filename, std::ios_base::binary);
	if (file) {
		file.seekg(0, std::ios_base::end);
		std::streamsize size = file.tellg();
		if (size > 0) {
			file.seekg(0, std::ios_base::beg);
			contents.resize(static_cast<size_t>(size));
			file.read(&contents[0], size);
		}
		contents.push_back('\0');
	}
	else {
		throw std::invalid_argument(std::string("The file ") + filename +
			" doesn't exists");
	}
}

}