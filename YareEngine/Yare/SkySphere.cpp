#include "SkySphere.hpp"
#include "Graphics/Renderer.hpp"
#include <GL/glew.h>


namespace yare{

	using namespace graphics;
	using namespace system;


	SkySphere::SkySphere(int radius , int sectors )
	{

		_sphereMesh.reset(new SphereMesh(radius, sectors));
		//Create the Skybox Shader
		std::string vertSource, fragSource;
		FileSystem::readFile(YARE_ASSET("Shaders/skybox.vert"), vertSource);
		FileSystem::readFile(YARE_ASSET("Shaders/skybox.frag"), fragSource);

		_shader.reset(Shader::Create());
		_shader->compile(vertSource, fragSource);

		//Load the Cubemap. Is a single texture formatted as such
		/*
			-----------------------
			|    | up  |     |     |
			|____|_____|_____|_____|
			|left|front|right|back |
			|____|_____|_____|_____|
			|    |down |     |     |
			|____|_____|_____|_____|

		*/



		TexturePixels pixels;
		TextureFormat format;
		Texture::ReadFile(YARE_ASSET("Images/skybox.png"), pixels);

		int faceWidth; int faceHeight;
		faceWidth = pixels.width / 4;
		faceHeight = pixels.height / 3;


		TexturePixels faces[6]; //hold onto if wanted to paint on?
		//right=0, left, up, down, back, front

		_texture.reset(Texture::Create(TextureType::CubeMap));
		TextureRegion region;
		region.width = faceWidth;
		region.height = faceHeight;
		TextureFace face;


		//load right face
		region.xoffset = faceWidth * 2;
		region.yoffset = faceHeight * 1;
		face = TextureFace::Right;
		Texture::ReadRegion(pixels, faces[(int)face], region);
		_texture->load(faces[(int)face], face);

		//load left face
		region.xoffset = faceWidth * 0;
		region.yoffset = faceHeight * 1;
		face = TextureFace::Left;
		Texture::ReadRegion(pixels, faces[(int)face], region);
		_texture->load(faces[(int)face], face);

		//load up face
		region.xoffset = faceWidth * 1;
		region.yoffset = faceHeight * 0;
		face = TextureFace::Top;
		Texture::ReadRegion(pixels, faces[(int)face], region);
		_texture->load(faces[(int)face], face);


		//load down face
		region.xoffset = faceWidth * 1;
		region.yoffset = faceHeight * 2;
		face = TextureFace::Bottom;
		Texture::ReadRegion(pixels, faces[(int)face], region);
		_texture->load(faces[(int)face], face);


		//load front face
		region.xoffset = faceWidth * 1;
		region.yoffset = faceHeight * 1;
		face = TextureFace::Front;
		Texture::ReadRegion(pixels, faces[(int)face], region);
		_texture->load(faces[(int)face], face);


		//load back face
		region.xoffset = faceWidth * 3;
		region.yoffset = faceHeight * 1;
		face = TextureFace::Back;
		Texture::ReadRegion(pixels, faces[(int)face], region);
		_texture->load(faces[(int)face], face);


		_texture->generateMipMaps();

		_sphereMesh->setTexture(_texture, 0);
		_sphereMesh->setShader(_shader);

		RenderState & state = _sphereMesh->getRenderCommand().state;
		state.cullFace = RenderCullFace::Front;

	}

	SkySphere::~SkySphere()
	{
	}

	void SkySphere::render(Renderer * renderer)
	{
		_sphereMesh->render(renderer);
	}

	glm::mat4& SkySphere::getModel()
	{
		return _sphereMesh->getModel();
	}
	void SkySphere::setModel(glm::mat4& model)
	{
		_sphereMesh->setModel(model);
	}



}
