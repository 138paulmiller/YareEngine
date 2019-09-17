#pragma once
#include "Shader.hpp"
#include "Texture.hpp"

#include <unordered_map>

/*
TODO - Use resource manager and only use Texture ID not the pointer

*/

namespace yare { 
	namespace graphics {  

	//Use a uniform block to block copy uniform names to texture bindings
	//Create Uniform Buffer Layout Struct. Use this and just create a single templated set(Struct * struct)
	//https://www.lighthouse3d.com/tutorials/glsl-tutorial/uniform-blocks/
	struct TextureBlock
	{
		std::unordered_map<std::string ,  Texture *> _textures;

		void setTexture(const std::string& name, Texture* texture);
		

		/*
		Load all uniforms into shader
		*/
		void load(Shader* shader);
	};

	} 
} 