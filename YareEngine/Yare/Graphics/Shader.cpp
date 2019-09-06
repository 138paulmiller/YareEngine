
#include "Shader.hpp"

#include "Platform.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLShader.hpp"


YARE_GRAPHICS_MODULE_BEG

Shader * Shader::Create()
{
	switch (Platform::Current().getRenderer())
	{
	case PlatformRenderer::OpenGL:
		return new OpenGLShader();
	default: 
		YARE_ASSERT(false, "No Render platform selected! "); 
		return 0;
	}
}

YARE_GRAPHICS_MODULE_END
