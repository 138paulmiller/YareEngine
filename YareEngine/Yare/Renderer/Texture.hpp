#pragma once

namespace yare 
{



enum class TextureFormat
{
	R8, RG8, RGB8, RGBA8,
};
class Texture
{
	public:
		~Texture() = default;
		virtual void load(TextureFormat sourceFormat, TextureFormat internalFormat, 
			void * data, 
			unsigned int size
		) = 0;

};
}
