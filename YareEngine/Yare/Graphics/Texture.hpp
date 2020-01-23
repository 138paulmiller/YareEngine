#pragma once

#include <string>
#include <iostream>

namespace yare 
{ 
	namespace graphics 
	{  


		enum class TextureFormat
		{
			None = 0,
			R8, RG8, RGB8, RGBA8, 
			XYZ16, //16 bit floating point
			XYZ32, //32 bit floating point 

		};
		enum class TextureType
		{
			Image = 0, //Default 2d image data
			Cubemap
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

		enum class TextureWrap
		{
			Clamp = 0, Repeat, Count
		};
		enum class TextureFilter
		{
			Linear = 0, Nearest, Count
		};


		struct TexturePixels
		{
			TexturePixels(
				unsigned char* data = 0,
				int width = 0, int height = 0,
				TextureFormat format = TextureFormat::None
			);
			~TexturePixels();
			unsigned char* data; //raw image data
			int width, height; //
			TextureFormat format; //format tpye and components
		};

		struct TextureRegion
		{
			TextureRegion(
				int width = 0, int height = 0,
				int xoffset = 0, int yoffset = 0
			);

			int width, height;
			int xoffset, yoffset;
		};


		class Texture
		{
			public:
				///////////////////////////////Statics ////////////////////////
				static Texture* Create(
					TextureType type = TextureType::Image, 
					TextureFormat internalFormat = TextureFormat::RGBA8
				);
				//Default loads RGBA8 image
				static Texture* CreateFromFile(const std::string& filepath);

				
				//
				static void ReadFile(const std::string & filepath, TexturePixels & pixels);
				static void ReadRegion(
					const TexturePixels & pixelsIn,
					TexturePixels & pixelsOut,
					const TextureRegion & region 
				);

				///////////////////////////////Methods ////////////////////////
				virtual ~Texture() = default;

				virtual void load(
					const TexturePixels & pixels,
					TextureFace face = TextureFace::Right, 
					int level = 0
				) = 0;
	
				virtual void update( TextureWrap wrap = TextureWrap::Clamp, 
									 TextureFilter filter = TextureFilter::Linear
				) = 0;

				virtual void generateMipMaps()=0;
				virtual void bind(unsigned int unit = 0) = 0;
				virtual void unbind() = 0;
					
				inline int getWidth() const {return _width; }
				inline int getHeight()const {return _height; }

			protected:
				inline void setWidth (int width) {  _width = width ; }
				inline void setHeight(int height) { _height= height; }
			private:
				int _width, _height;
		};
	} 
} 