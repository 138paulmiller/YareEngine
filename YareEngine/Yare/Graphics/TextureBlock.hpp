#include "Shader.hpp"
#include "Texture.hpp"

#include <unordered_map>

YARE_GRAPHICS_MODULE_BEG

//Use a uniform block to block copy uniform names to texture bindings
//Create Uniform Buffer Layout Struct. Use this and just create a single templated set(Struct * struct)
//https://www.lighthouse3d.com/tutorials/glsl-tutorial/uniform-blocks/
struct TextureBlock
{
	std::unordered_map<std::string ,  Texture *> textures;

	void setTexture(const std::string& name, Texture * texture)
	{
		textures[name] = texture;
		
	}

	void load(Shader* shader)
	{
		int i = 0;
		Texture* texture;
		const  std::string * name;
		for (std::pair<std::string ,  Texture * > pair : textures)
		{
			texture = pair.second;
			name = &pair.first;
			//bind texture to unit i
			texture->bind(i);
			//set sampler to this unit
			shader->setUniform(*name, i);
			//next unit
			i++;
		}
	}
};

YARE_GRAPHICS_MODULE_END