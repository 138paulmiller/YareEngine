
#pragma once
#include <iostream>
#include <Yare/Graphics/Graphics.hpp>

YARE_GRAPHICS_MODULE_BEG

// Ask Opengl for errors:
void _OpenGLCheckError(const char* file, unsigned int line, std::ostream & out = std::cout);

#ifdef YARE_DEBUG
#define OpenGLCheckError() _OpenGLCheckError(__FILE__,__LINE__)
#else 
#define OpenGLCheckError() 

#endif 

YARE_GRAPHICS_MODULE_END

