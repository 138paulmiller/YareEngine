#include "OpenGLTexture.hpp"
#include "OpenGLCommon.hpp"
#include <Yare/Renderer/Error.hpp>
namespace yare
{

	OpenGLTexture::OpenGLTexture(TextureType type, TextureFormat internalFormat )
	{
		_texture = 0;
		_unit = 0;    //unit it was last bound to
		_isLoaded = false;

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
		TextureFormat sourceFormat,
		const void * data,  int level,
		 int width,  int height,
		 int face,
		 int xoff,  int yoff
	)	
	{
		
		switch (sourceFormat)
		{
		case TextureFormat::R8: 
			loadData(GL_RED, GL_UNSIGNED_BYTE, data, level, width, height, face, xoff, yoff);
			break;
		case TextureFormat::RG8:
			loadData(GL_RG, GL_UNSIGNED_BYTE, data, level, width, height, face, xoff, yoff);
			break;
		case TextureFormat::RGB8:
			loadData(GL_RGB, GL_UNSIGNED_BYTE, data, level, width, height, face, xoff, yoff);
			break;
		}

		_isLoaded = true;
	
	}
	void OpenGLTexture::loadData(
		unsigned int sourceFormat, unsigned int sourceType,
		const void * data, int level,
		 int width,  int height, 
		 int face,
		 int xoff,  int yoff
	)
	{
		unsigned int target = 
			GL_TEXTURE_CUBE_MAP 
				? (GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
				: _target;

		if (_isLoaded)
		{
				
			glTexSubImage2D(
				_target + face,
				level,
				xoff, yoff,
				width, height,
				sourceFormat, sourceType, data);

		}
		else
		{
			glTexImage2D(_target, level, _internalFormat, width, height, 0, sourceFormat, sourceType, data);
			glGenerateMipmap(_target);
			OpenGLCheckError();
		}
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
}