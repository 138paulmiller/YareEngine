
#include "Shader.hpp"

#include "Renderer.hpp"

#include "OpenGL/OpenGLShader.hpp"


YARE_GRAPHICS_MODULE_BEG

Shader * Shader::Create()
{
	switch (Renderer::GetInstance()->getAPI())
	{
	case RenderAPI::OpenGL:
		return new OpenGLShader();
	default: 
		YARE_ASSERT(false, "No Render platform selected! "); 
		return 0;
	}
}

YARE_GRAPHICS_MODULE_END
