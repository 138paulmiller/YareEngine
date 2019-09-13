
#pragma once
#include <string>


//YARE_PROJECT_DIR  defd by cmake
#define YARE_ASSET(path) YARE_PROJECT_DIR "/Assets/" path


namespace yare { namespace os { 

//TODO - this object can be used to abstract filestem commands. 
// Such as mkdir, cd, read/write etc...
class FileSystem
{
public:
	// file reading 
	static void readFile(const std::string& filename, std::string& contents);

};

} }
