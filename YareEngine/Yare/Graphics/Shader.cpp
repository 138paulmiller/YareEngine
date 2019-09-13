
#include "Shader.hpp"

#include "Renderer.hpp"

#include "OpenGL/OpenGLShader.hpp"


namespace yare { namespace graphics {  

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

} } 
