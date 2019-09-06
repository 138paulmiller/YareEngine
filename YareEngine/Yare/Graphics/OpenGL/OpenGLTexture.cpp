#include "OpenGLTexture.hpp"
#include "OpenGLCommon.hpp"
#include <Yare/Graphics/Texture.hpp>

YARE_GRAPHICS_MODULE_BEG

OpenGLTexture::OpenGLTexture(TextureType type, TextureFormat internalFormat )
{
	_texture = 0;
	_unit = 0;    //unit it was last bound to
		
	for(int i = 0; i < (int)TextureFace::Count; i++)
	{
		_isLoadedMap[i] = false;
	}
	switch (type)
	{
	case TextureType::Image:
		_target = GL_TEXTURE_2D;
		break;
	case TextureType::CubeMap:
		_target = GL_TEXTURE_CUBE_MAP;
		break;
	default:
		YARE_ASSERT(false, "Incorrect Texture format");
		return;
	}

	switch (internalFormat)
	{
	case TextureFormat::R8:
		_internalFormat = GL_R8;
		break;

	case TextureFormat::RG8:
		_internalFormat = GL_RG8;
		break;
	case TextureFormat::RGB8:
		_internalFormat = GL_RGB8;
		break;		
	case TextureFormat::RGBA8:
		_internalFormat = GL_RGBA8;
		break;
	default :
		YARE_ASSERT(false, "Incorrect Texture format");
			
		return;
	}

	glGenTextures(1, &_texture);
	glBindTexture(_target, _texture);
}
	
OpenGLTexture::~OpenGLTexture()
{
	glBindTexture(_target, 0);
	glDeleteTextures(1, &_texture);
}
	
void OpenGLTexture::load(
	const TexturePixels & pixels, 
	TextureFace face ,
	int level 
)
{

	unsigned int format, type = GL_UNSIGNED_BYTE;
	switch (pixels.format)
	{
	case TextureFormat::R8: 
		format = GL_RED;
		break;
	case TextureFormat::RG8:
		format = GL_RG;
		break;
	case TextureFormat::RGB8:
		format = GL_RGB;
		break;
		
	case TextureFormat::RGBA8:
		format = GL_RGBA;
		break;
	default:
		YARE_LOG("Invalid format!");
		return;
	}
	unsigned int target = 
		_target == GL_TEXTURE_CUBE_MAP 
			? (GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)(face))
			: _target;


	glBindTexture(_target, _texture);

	if (_isLoadedMap[(int)(face)])
	{	
		glTexSubImage2D(
			target ,
			level,
			0, 0, //offsets
			pixels.width, pixels.height,
			format, type, pixels.data);
	}
	else
	{

		_isLoadedMap[(int)(face)] = true;
		glTexImage2D(target, level, 
			_internalFormat, 
			pixels.width, pixels.height,
			0,//border 
			format, type, pixels.data);

		OpenGLCheckError();
	}

	glBindTexture(_target, 0);
}

void OpenGLTexture::generateMipMaps()
{

	glBindTexture(_target, _texture);
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(_target);

}
void OpenGLTexture::bind(unsigned int unit )
{
	_unit = unit;
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(_target, _texture);
}

void OpenGLTexture::unbind()
{
	glBindTexture(_target, 0);
}

YARE_GRAPHICS_MODULE_END