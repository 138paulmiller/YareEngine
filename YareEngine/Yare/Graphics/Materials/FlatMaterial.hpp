#pragma once 
#include "../Material.hpp"

namespace yare {
	namespace graphics {

		class FlatMaterial : public Material
		{
		public:
			~FlatMaterial() ;
			//Load uniform/textures into uniforms/texture block
			void unloadUniforms(UniformBlock& uniforms) override ;
			void loadTextures(TextureBlock& textures) override ;

			inline glm::vec3 getBase()     { return _base; }

			inline void setBase(const glm::vec3 base) { _base= base; }

		private:
			glm::vec3 _base;
			
		};
	}
}