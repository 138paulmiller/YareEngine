#include "SkyBox.hpp"
#include <Yare/Geometry/Box.hpp>
#include <Yare/AssetManager.hpp>

namespace yare {
	
	namespace graphics {



		SkyBox::SkyBox()
		{

			Mesh::loadVertexArray(geometry::Box::CreateVertexArray({ 1,1,1 }));

			//Create the Skybox Shader

			_shader= AssetManager::GetInstance().get<Shader>("Shader_SkyBox");

			Mesh::command.shader = _shader;
			Mesh::command.state.cullFace = RenderCullFace::Front;
			Mesh::command.lighting = RenderLighting::Unlit;
			Mesh::command.mode = RenderMode::Mesh;//Box is not indexed

		}

		SkyBox::~SkyBox()
		{
		}

		void SkyBox::setCubemap(Texture* cubemap)
		{
			_cubemap = cubemap;
			Mesh::command.textures.setTexture("environment", _cubemap);
		}

		void SkyBox::loadFace(const TexturePixels& pixels, const TextureFace& face)
		{
			_cubemap->load(pixels, face);
			_cubemap->generateMipMaps();

		}

	}
}