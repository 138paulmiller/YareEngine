#pragma once
#include <Yare/Renderer/Texture.hpp>
#include <memory>
namespace yare
{

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(TextureType type = TextureType::Image, TextureFormat internalFormat = TextureFormat::RGBA8);
	~OpenGLTexture();

	void load(
		TextureFormat sourceFormat, 
		const void * data,  int level,
		 int width,  int height,
		 int face = 0,
		 int xoff = 0,  int yoff = 0) ;



	void bind(unsigned int unit = 0)override;
	void unbind() override;
protected : 
private:
	void loadData(
		unsigned int sourceFormat, unsigned int sourceType,
		const void * data,  int level,
		 int width, int height, 
		 int face,
		 int xoff,  int yoff 
	);

	unsigned int _isLoaded; //if data has been loaded

	unsigned int _texture; //handle to the texture object
	unsigned int _target; //texture type
	unsigned int _internalFormat; //
	unsigned int _unit;    //unit it was last bound to
	
};


}