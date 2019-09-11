
#include "Texture.hpp"

#include "Renderer.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

YARE_GRAPHICS_MODULE_BEG

static int FormatToComponentCount(TextureFormat format){
	switch (format)
	{
	case TextureFormat::RGBA8:
		return 4;
	case TextureFormat::RGB8:
		return 3;
	case TextureFormat::RG8:
		return 2;
	case TextureFormat::R8:
		return 1;
	default:
		YARE_LOG("Format Not Supported" );
		break;

	}
};

TexturePixels::TexturePixels(
	unsigned char* data,
	int width, int height,
	TextureFormat format
)
	:
	data(data),
	width(width), height(height),
	format(format)
{}
TexturePixels::~TexturePixels()
{
	delete data;
}

TextureRegion::TextureRegion(
	int width, int height ,
	int xoffset,int yoffset
):
	width(width  ),  height(height),
	xoffset(xoffset),  yoffset(yoffset)		
{}
		
Texture* Texture::Create(TextureType type , TextureFormat internalFormat)
{
	switch (Renderer::GetInstance()->getAPI())
	{
	case RenderAPI::OpenGL:
		return new  OpenGLTexture(type, internalFormat);
	default:
		YARE_ASSERT(false, "No Render platform selected! "); 
		return 0;
	}
}

void Texture::ReadFile(const std::string & filepath, TexturePixels & pixels)
{
	int channels;
	stbi_set_flip_vertically_on_load(true);
	pixels.data = stbi_load(filepath.c_str(), &pixels.width, &pixels.height, &channels, 0);
	if (pixels.data)
	{
		switch (channels)
		{
		case 4:
			pixels.format = TextureFormat::RGBA8;

			break;
		case 3:
			pixels.format = TextureFormat::RGB8;
			break;
		case 2:
			pixels.format = TextureFormat::RG8;
			break;
		case 1:
			pixels.format = TextureFormat::R8;
			break;
		default : 
				YARE_LOG("Could not read " << filepath.c_str());
			break;

		}
	}
	else
	{
		pixels.data=0;
		std::cout << "Error Could not read texture file " << filepath.c_str() << std::endl;
	}

}

void Texture::ReadRegion(
	const TexturePixels & pixelsIn,
	TexturePixels & pixelsOut,
	const TextureRegion & region
)
{
	if ((region.width >= region.width + pixelsIn.width)
		|| (region.width >= region.width + pixelsIn.width))
	{
		YARE_LOG("Read Region: Invalid Region!");
		return;
	}
		//
	int comps = FormatToComponentCount(pixelsIn.format);
		
	int size = pixelsIn.height * pixelsIn.width * comps;
		
	delete pixelsOut.data;
		
	pixelsOut.data = new unsigned char[size];
	pixelsOut.width = region.width;
	pixelsOut.height = region.height;
	pixelsOut.format= pixelsIn.format;

	int indexIn, indexOut;
	for(int x = 0; x < region.width ; x++)
		for (int y = 0; y < region.height ; y++)
		{
			for (int c = 0; c < comps; c++) {
				indexIn = (pixelsIn.width * (region.yoffset+y) + region.xoffset + x) * comps;
				indexOut = (region.width * y + x) * comps;
				pixelsOut.data[indexOut + c] = pixelsIn.data[ indexIn + c];
			}
		}

}

YARE_GRAPHICS_MODULE_END