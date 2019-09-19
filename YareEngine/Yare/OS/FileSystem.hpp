
#pragma once
#include <string>


//YARE_PROJECT_DIR  defd by cmake



namespace yare { namespace os { 

//TODO - this object can be used to abstract filestem commands. 
// Such as mkdir, cd, read/write etc...
class FileSystem
{
public:
	// file reading 
	static void ReadFile(const std::string& filename, std::string& contents);

};

} }
