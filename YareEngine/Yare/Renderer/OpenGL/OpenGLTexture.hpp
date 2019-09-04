#pragma once
#include <Yare/Renderer/Texture.hpp>
#include <memory>
namespace yare
{

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture();
	virtual ~OpenGLTexture();

	virtual void load(
		TextureFormat sourceFormat,
		TextureFormat internalFormat,
		void* data,
		unsigned int size) override;

	virtual void bind(unsigned int unit = 0)override;
	virtual void unbind() override;
protected : 
private:
	unsigned int _texture; //handle to the texture object
	unsigned int _unit;    //unit it was last bound to
	
};


}