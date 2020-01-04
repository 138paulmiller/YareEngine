#include "PhongMaterial.hpp"
namespace yare {
	namespace graphics {
		
		PhongMaterial::~PhongMaterial()
		{
			_diffuseTexture =  nullptr;
			_specularTexture=  nullptr;
			_emissiveTexture=  nullptr;

		}
		//Load uniform/textures into uniforms/texture block
		void PhongMaterial::unloadUniforms(UniformBlock& uniforms)
		{
			uniforms.setUniform("material.shininess", _shininess);
		}
		
		void PhongMaterial::loadTextures(TextureBlock& textures)
		{

				textures.setTexture("material.diffuse", _diffuseTexture);
				textures.setTexture("material.specular", _specularTexture);
				textures.setTexture("material.emissive", _emissiveTexture);
		}

		//Create a switch for using either map or float3

	}
}