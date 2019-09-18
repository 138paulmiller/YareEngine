#include "SkyBox.hpp"
#include "Renderer.hpp"
#include "../Geometry/Sphere.hpp"
#include "../Geometry/Box.hpp"
#include "../OS/FileSystem.hpp"
#include "../AssetManager.hpp"

namespace yare {
	using namespace geometry;
	using namespace os;

	namespace graphics {



		SkyBox::SkyBox()
		{

			Mesh::loadVertexArray(Box::CreateVertexArray({ 1,1,1 }));

			//Create the Skybox Shader

			_shader=AssetManager::GetInstance().get<Shader>("Shader_SkyBox");

			Mesh::renderData.shader = _shader;
			Mesh::renderData.state.cullFace = RenderCullFace::Front;

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