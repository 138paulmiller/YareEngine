#include "SkyBox.hpp"
#include "Graphics/Renderer.hpp"
#include "Geometry/Sphere.hpp"
#include "Geometry/Box.hpp"


namespace yare {

	using namespace graphics;
	using namespace geometry;
	using namespace os;


	SkyBox::SkyBox()
	{

		Mesh::loadVertexArray(Box::CreateVertexArray({1,1,1}));

		//Create the Skybox Shader
		std::string vertSource, fragSource;
		FileSystem::readFile(YARE_ASSET("Shaders/skybox.vert"), vertSource);
		FileSystem::readFile(YARE_ASSET("Shaders/skybox.frag"), fragSource);

		_shader.reset(Shader::Create());
		_shader->compile(vertSource, fragSource);

		Mesh::renderData.shader = _shader.get();
		Mesh::renderData.state.cullFace = RenderCullFace::Front;

	}

	SkyBox::~SkyBox()
	{
	}

	void SkyBox::preRender()
	{
		//if environment map is dirty, rebind
		Mesh::preRender();
	}
	
	void SkyBox::postRender()
	{
		Mesh::postRender();
	}
	void SkyBox::setCubemap(Texture* cubemap) 
	{ 
		_cubemap.reset(cubemap); 
		Mesh::renderData.textures.setTexture("environment", _cubemap.get());
	}

	void SkyBox::loadFace(const TexturePixels & pixels, const TextureFace& face)
	{
		_cubemap->load(pixels, face);
		_cubemap->generateMipMaps();

	}

}
