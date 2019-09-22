#include "FlatMaterial.hpp"
namespace yare {
	namespace graphics {
		
		FlatMaterial::~FlatMaterial()
		{
			_base = glm::vec3(1, 0, 0);
		}
		//Load uniform/textures into uniforms/texture block
		void FlatMaterial::loadUniforms(UniformBlock& uniforms)
		{
			uniforms.setUniform("material.base", _base);
		}
		
		void FlatMaterial::loadTextures(TextureBlock& textures)
		{
		}

		//Create a switch for using either map or float3

	}
}