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
		void PhongMaterial::loadUniforms(UniformBlock& uniforms)
		{
			uniforms.setUniform("material.shininess", _shininess);
		}
		
		void PhongMaterial::loadTextures(TextureBlock& textures)
		{

			if(_diffuseTexture.get())
				textures.setTexture("material.diffuse", _diffuseTexture.get());
			if(_specularTexture.get())
				textures.setTexture("material.specular", _specularTexture.get());
			if(_emissiveTexture.get())
				textures.setTexture("material.emissive", _emissiveTexture.get());
		}

		//Create a switch for using either map or float3

	}
}