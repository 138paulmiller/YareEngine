#pragma once

#include <string>
#include <iostream>
#include "Graphics.hpp"

YARE_GRAPHICS_MODULE_BEG


enum class TextureFormat
{
	None = 0,
	R8, RG8, RGB8, RGBA8
};
enum class TextureType
{
	Image = 0, //Default 2d image data
	CubeMap

};

struct TexturePixels
{
	TexturePixels(
		unsigned char* data=0,
		int width=0, int height = 0,
		TextureFormat format = TextureFormat::None
	);
	~TexturePixels();
	unsigned char * data; //raw image data
	int width, height; //
	TextureFormat format; //format tpye and components
};

struct TextureRegion
{
	TextureRegion(
	int width  =0, int height  = 0,
	int xoffset=0, int yoffset = 0
	);

	int width, height;
	int xoffset, yoffset;
};


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
enum class TextureFace
{
	Right=0, Left, Top, Bottom, Front, Back, Count
};

class Texture
{
	public:
		static Texture* Create(TextureType type = TextureType::Image, TextureFormat internalFormat = TextureFormat::RGBA8);

		static void ReadFile(const std::string & filepath, TexturePixels & pixels);
		static void ReadRegion(
			const TexturePixels & pixelsIn,
			TexturePixels & pixelsOut,
			const TextureRegion & region 
		);

		~Texture() = default;


		virtual void load(
			const TexturePixels & pixels,
			TextureFace face = TextureFace::Right, 
			int level = 0
		) = 0;
	
		virtual void generateMipMaps()=0;
		virtual void bind(unsigned int unit = 0) = 0;
		virtual void unbind() = 0;

};
YARE_GRAPHICS_MODULE_END