#include "PhongMaterial.hpp"
#include "../AssetManager.hpp"
namespace yare {
	namespace graphics {
		PhongMaterial::PhongMaterial()
		{
			RenderCommand & command = getCommand();
			command.shader = AssetManager::GetInstance().get<Shader>("phong_mesh");
			command.mode = RenderMode::Mesh;
			command.lighting = RenderLighting::Surface;

			_diffuseTexture = nullptr;
			_specularTexture = nullptr;
			_emissiveTexture = nullptr;
			_shininess = 1;

		}
		PhongMaterial::~PhongMaterial()
		{
			//does not own textures
		}
		//Load uniform/textures into uniforms/texture block
		void PhongMaterial::unloadUniforms(UniformBlock& uniforms)
		{
			uniforms.setUniform("material.shininess", _shininess);
			if(_diffuseTexture)
			uniforms.setUniform("material.diffuse", _diffuseTexture);

			if(_specularTexture)
				uniforms.setUniform("material.specular", _specularTexture);
			if(_emissiveTexture)
				uniforms.setUniform("material.emissive", _emissiveTexture);
		}
		

		//Create a switch for using either map or float3

	}
}