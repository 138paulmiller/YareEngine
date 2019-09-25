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

			Mesh::renderData.shader = _shader;
			Mesh::renderData.state.cullFace = RenderCullFace::Front;
			Mesh::renderData.lighting = RenderLighting::Unlit;

		}

		SkyBox::~SkyBox()
		{
		}

		void SkyBox::setCubemap(Texture* cubemap)
		{
			_cubemap = cubemap;
			Mesh::renderData.textures.setTexture("environment", _cubemap);
		}

		void SkyBox::loadFace(const TexturePixels& pixels, const TextureFace& face)
		{
			_cubemap->load(pixels, face);
			_cubemap->generateMipMaps();

		}

	}
}