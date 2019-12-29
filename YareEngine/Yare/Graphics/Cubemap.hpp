#pragma once

#include "Texture.hpp"

namespace yare { 
	namespace graphics {  

		class Cubemap : public Texture
		{
		public:
			static Texture* Create(TextureFormat internalFormat = TextureFormat::RGBA8);
			//Default loads RGBA8 image
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

			static Texture* CreateFromFile(const std::string& filepath);

		};

	} 
} 