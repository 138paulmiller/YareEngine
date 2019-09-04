
#include "Shader.hpp"

#include "Platform.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLShader.hpp"


namespace yare
{

Shader * Shader::Create()
{
	switch (Platform::Current().getRenderer())
	{
	case PlatformRenderer::OpenGL:
		return new OpenGLShader();
	default: 
		YARE_ASSERT(false, "No Render platform selected! "); return 0;
	}
}

}
