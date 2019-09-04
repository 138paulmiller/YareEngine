
#pragma once
#include <string>


namespace yare
{
	//TODO - this object can be used to abstract filestem commands. 
	// Such as mkdir, cd, read/write etc...
	class FileSystem
	{
	public:
		// file reading 
		static void readFile(const std::string& filename, std::string& contents);

	};
}