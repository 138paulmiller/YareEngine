
#include "Cubemap.hpp"

namespace yare { namespace graphics {  
Texture* Cubemap::Create(TextureFormat internalFormat)
{
	return Texture::Create(TextureType::Cubemap, internalFormat);
}

Texture* Cubemap::CreateFromFile(const std::string& filepath)
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
	texture->update();
	return texture;

}

} } 