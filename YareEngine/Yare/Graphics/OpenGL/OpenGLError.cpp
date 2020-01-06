
#include "OpenGLError.hpp"

#include "OpenGLCommon.hpp"


namespace yare { namespace graphics {  




	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		return;
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
		fprintf(stderr, "END");

	}




void _OpenGLCheckError(const char* file, unsigned int line, std::ostream & out )
{  GLenum errorCode = glGetError();


// During init, enable debug output
glEnable(GL_DEBUG_OUTPUT);
glDebugMessageCallback(MessageCallback, 0);

  while (errorCode != GL_NO_ERROR) {
    std::string fileString(file);
	std::string error = "unknown error";

    // clang-format off
    switch (errorCode) {
      case GL_INVALID_ENUM:      error = "GL_INVALID_ENUM"; break;
      case GL_INVALID_VALUE:     error = "GL_INVALID_VALUE"; break;
      case GL_INVALID_OPERATION: error = "GL_INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:    error = "GL_STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:   error = "GL_STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:     error = "GL_OUT_OF_MEMORY"; break;
    }
    // clang-format on

	out << "OpenglError : file="<< std::endl << file << " line=" << line
         << std::endl << "[" << error << "]" <<std::endl;
    errorCode = glGetError();
  }
}


} } 