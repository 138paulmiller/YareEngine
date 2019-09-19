#pragma once

#include "Texture.hpp"

namespace yare { 
	namespace graphics {  

		class Cubemap : public Texture
		{
		public:
			static Texture* Create(TextureFormat internalFormat = TextureFormat::RGBA8);
			//Default loads RGBA8 image
			static Texture* CreateFromFile(const std::string& filepath);

		};

	} 
} 