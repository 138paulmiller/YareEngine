
#include "Texture.hpp"

#include "Graphics.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace yare { namespace graphics {  

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
	switch (Graphics::GetRenderAPI())
	{
	case RenderAPI::OpenGL:
		return new  OpenGLTexture(type, internalFormat);
	default:
		YARE_ASSERT(false, "No Render platform selected! "); 
		return 0;
	}
}
Texture* Texture::CreateFromFile(const std::string& filepath)
{
	TexturePixels pixels;
	Texture::ReadFile(filepath, pixels);
	
	Texture* texture = Texture::Create();
	texture->load(pixels);
	texture->generateMipMaps();
	return texture;
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
		|| (region.height >= region.height + pixelsIn.height))
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
				pixelsOut.data[indexOut + c] = (char)pixelsIn.data[ indexIn + c];
			}
		}

}
Texture* Texture::CreateCubemapFromFile(const std::string& filepath)
{
	TexturePixels pixels;
	TextureFormat format;
	Texture::ReadFile(filepath, pixels);
	Texture * texture = Texture::Create(TextureType::Cubemap);
	int faceWidth; int faceHeight;
	faceWidth = pixels.width / 4;
	faceHeight = pixels.height / 3;


	TexturePixels faces[6]; //hold onto if wanted to paint on?
	//right=0, left, up, down, back, front

	TextureRegion region;
	region.width = faceWidth;
	region.height = faceHeight;
	TextureFace face;


	//load right face
	region.xoffset = faceWidth * 2;
	region.yoffset = faceHeight * 1;
	face = TextureFace::Right;
	Texture::ReadRegion(pixels, faces[(int)face], region);
	texture->load(faces[(int)face], face);

	//load left face
	region.xoffset = faceWidth * 0;
	region.yoffset = faceHeight * 1;
	face = TextureFace::Left;
	Texture::ReadRegion(pixels, faces[(int)face], region);
	texture->load(faces[(int)face], face);

	//load up face
	region.xoffset = faceWidth * 1;
	region.yoffset = faceHeight * 0;
	face = TextureFace::Top;
	Texture::ReadRegion(pixels, faces[(int)face], region);
	texture->load(faces[(int)face], face);


	//load down face
	region.xoffset = faceWidth * 1;
	region.yoffset = faceHeight * 2;
	face = TextureFace::Bottom;
	Texture::ReadRegion(pixels, faces[(int)face], region);
	texture->load(faces[(int)face], face);


	//load front face
	region.xoffset = faceWidth * 1;
	region.yoffset = faceHeight * 1;
	face = TextureFace::Front;
	Texture::ReadRegion(pixels, faces[(int)face], region);
	texture->load(faces[(int)face], face);


	//load back face
	region.xoffset = faceWidth * 3;
	region.yoffset = faceHeight * 1;
	face = TextureFace::Back;
	Texture::ReadRegion(pixels, faces[(int)face], region);
	texture->load(faces[(int)face], face);
	
	texture->generateMipMaps();
	return texture;

}

} } 