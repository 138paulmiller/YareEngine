#pragma once
#include "OpenGLCommon.hpp"

#include <Yare/Graphics/Texture.hpp>

namespace yare { namespace graphics {  

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(
		TextureType type = TextureType::Image, 
		TextureFormat internalFormat = TextureFormat::RGBA8
	);
	
	~OpenGLTexture();

	void load(
		const TexturePixels & pixels, 
		TextureFace face = TextureFace::Right,
		int level = 0
	)override;


	void update(
		TextureWrap wrap = TextureWrap::Clamp, 
		TextureFilter filter = TextureFilter::Linear
	)override;

	void generateMipMaps() override;
	void bind(unsigned int unit = 0)override;
	void unbind() override;

	inline int getHandle()const { return _texture; }
protected : 
private:
	bool _isLoadedMap[(int)TextureFace::Count]; //if data has been loaded

	unsigned int _texture; //handle to the texture object
	unsigned int _target; //texture type
	unsigned int _internalFormat; //opengl internal format
	unsigned int _unit;    //unit it was last bound to
	
};


} } 