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
		}
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
			uniforms.setUniform("material.diffuse", _diffuseTexture);
			uniforms.setUniform("material.specular", _specularTexture);
			uniforms.setUniform("material.emissive", _emissiveTexture);
		}
		

		//Create a switch for using either map or float3

	}
}