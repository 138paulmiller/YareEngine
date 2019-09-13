
#include "TextureBlock.hpp"

/*
TODO - Use resource manager and only use Texture ID not the pointer

*/

namespace yare {
	namespace graphics {

		void TextureBlock::setTexture(const std::string& name, Texture* texture)
		{
			_textures[name] = texture;

		}

		void TextureBlock::load(Shader* shader)
		{
			int i = 0;
			Texture* texture;
			const  std::string* name;
			for (std::pair<std::string, Texture* > pair : _textures)
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

	}
}