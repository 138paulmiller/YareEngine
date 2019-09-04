
#pragma once
#include <iostream>
// Ask Opengl for errors:
void _OpenGLCheckError(const char* file, unsigned int line, std::ostream & out = std::cout);

#ifdef YARE_DEBUG
#define OpenGLCheckError() _OpenGLCheckError(__FILE__,__LINE__)
#else 
#define OpenGLCheckError() 

#endif 