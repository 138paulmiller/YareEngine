
#include "Shader.hpp"

#include "Graphics.hpp"

#include "OpenGL/OpenGLShader.hpp"


namespace yare { 
	namespace graphics {  

Shader * Shader::Create()
{
	switch (Graphics::GetRenderAPI())
	{
	case RenderAPI::OpenGL:
		return new OpenGLShader();
	default: 
		YARE_ASSERT(false, "No Render platform selected! "); 
		return 0;
	}
}

} } 
