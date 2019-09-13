
#pragma once
#include <iostream>

namespace yare { namespace graphics {  

// Ask Opengl for errors:
void _OpenGLCheckError(const char* file, unsigned int line, std::ostream & out = std::cout);

#ifdef YARE_DEBUG
#define OpenGLCheckError() _OpenGLCheckError(__FILE__,__LINE__)
#else 
#define OpenGLCheckError() 

#endif 

} } 

