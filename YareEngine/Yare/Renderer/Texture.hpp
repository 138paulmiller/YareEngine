#pragma once

#include <string>
#include <iostream>

namespace yare 
{

	enum class TextureFormat
	{
		R8=0, RG8, RGB8, RGBA8
	};
	enum class TextureType
	{
		Image = 0, //Default 2d image data
		CubeMap

	};


	class Texture
	{
		public:
			static Texture* Create(TextureType type = TextureType::Image, TextureFormat internalFormat = TextureFormat::RGBA8);

			static void ReadFile(const std::string & filepath, unsigned char ** data, int & width, int & height, TextureFormat & format);

			~Texture() = default;
			
			/*
			Cubmap_n
				  ------     +y
				 /  y / |    |__ +x
				------ x|   /
				| z  | /   +z
				|____|/
				x is right(+) left(-)
				y is up(+) down(-)
				z is front(+) back(-)
			- Faces are in order right, left, up, down, back, front
			*/

			virtual void load(
				TextureFormat sourceFormat,  
				const void * data,    int level,
				int width,   int height,
				int face = 0,
				int xoff = 0, int yoff = 0
			) = 0;
	
			virtual void bind(unsigned int unit = 0) = 0;
			virtual void unbind() = 0;
	};

}
