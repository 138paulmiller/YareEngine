#include "FlatMaterial.hpp"
#include "../AssetManager.hpp"
namespace yare {
	namespace graphics {
		FlatMaterial::FlatMaterial()
		{
			RenderCommand & command = getCommand();
			command.shader = AssetManager::GetInstance().get<Shader>("unlit_mesh");
			command.mode = RenderMode::IndexedMesh;
			command.lighting = RenderLighting::Unlit;
		}
		FlatMaterial::~FlatMaterial()
		{
			_base = glm::vec3(1, 0, 0);
		}
		//Load uniform/textures into uniforms/texture block
		void FlatMaterial::unloadUniforms(UniformBlock& uniforms)
		{
			uniforms.setUniform("material.base", _base);
		}
		

		//Create a switch for using either map or float3

	}
}