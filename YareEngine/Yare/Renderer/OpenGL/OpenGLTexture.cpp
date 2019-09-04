#include "OpenGLTexture.hpp"
#include "OpenGLCommon.hpp"
#include <Yare/Renderer/Error.hpp>
namespace yare
{

	OpenGLTexture::OpenGLTexture()
	{}
	
	OpenGLTexture::~OpenGLTexture()
	{}

	void OpenGLTexture::load(
		TextureFormat sourceFormat,
		TextureFormat internalFormat,
		void* data,
		unsigned int size)
	{}

	void OpenGLTexture::bind(unsigned int unit )
	{}

	void OpenGLTexture::unbind()
	{}
}