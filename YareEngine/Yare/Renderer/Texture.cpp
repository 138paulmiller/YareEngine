
#include "Texture.hpp"

#include "Platform.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace yare
{


	Texture* Texture::Create(TextureType type , TextureFormat internalFormat)
	{
		switch (Platform::Current().getRenderer())
		{
		case PlatformRenderer::OpenGL: 
			return new  OpenGLTexture(type, internalFormat);
		default:
			YARE_ASSERT(false, "No Render platform selected! "); 
			return 0;
		}
	}
	void Texture::ReadFile(const std::string & filepath, unsigned char ** data, int & width, int & height, TextureFormat & format)
	{
		int channels;
		*data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			if (channels == 4)
			{
				format = TextureFormat::RGBA8 ;
			}else if (channels == 3)
			{
				format = TextureFormat::RGB8;
			}
			else if (channels == 2)
			{
				format = TextureFormat::RG8;
			}
			else if (channels == 1)
			{
				format = TextureFormat::R8;
			}
		}
		else
		{
			delete data;
			std::cout << "Error Could not read texture file " << filepath.c_str() << std::endl;
		}

	}
}