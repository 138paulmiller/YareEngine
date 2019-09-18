#pragma once

#include <string>
#include <iostream>

namespace yare { namespace graphics {  


enum class TextureFormat
{
	None = 0,
	R8, RG8, RGB8, RGBA8
};
enum class TextureType
{
	Image = 0, //Default 2d image data
	Cubemap

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
	 /    /|    |__ +x
	------ |   /
	|    | /   +z
	|____|/
	x is right(+) left(-)
	y is top(+) bottom(-)
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
		//Default loads RGBA8 image
		static Texture* CreateFromFile(const std::string& filepath);

		//Creates and loads a Cubemapped raster formatted as such
		/*
			-----------------------
			|    | top |     |     |
			|____|_____|_____|_____|
			|left|front|right|back |
			|____|_____|_____|_____|
			|    |bottom     |     |
			|____|_____|_____|_____|

		*/

		static Texture* CreateCubemapFromFile(const std::string& filepath);

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
} } 