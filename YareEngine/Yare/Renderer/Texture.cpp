
#include "Texture.hpp"

#include "Platform.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLTexture.hpp"
namespace yare
{

	Texture* Texture::Create()
	{
		switch (Platform::Current().getRenderer())
		{
		case PlatformRenderer::OpenGL: 
			return new  OpenGLTexture();
		default:
			YARE_ASSERT(false, "No Render platform selected! "); return 0;
		}
	}

}