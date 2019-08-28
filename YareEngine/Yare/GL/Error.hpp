

#pragma once

// Ask Opengl for errors:
// Result is printed on the standard output
// usage :
//      glCheckError(__FILE__,__LINE__);
void _glCheckError(const char* file, unsigned int line);
#define glCheckError() _glCheckError(__FILE__,__LINE__)
