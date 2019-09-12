#include "SkyBox.hpp"
#include "Graphics/Renderer.hpp"
#include "Geometry/Sphere.hpp"
#include "Geometry/Box.hpp"


namespace yare{

	using namespace graphics;
	using namespace geometry;
	using namespace system;


	SkyBox::SkyBox(int radius , int sectors )
	{

		_mesh.reset(new Mesh());

		//use sphere
		
		//_mesh->loadVertexArray(Sphere::CreateVertexArray(20, 20, 20));
		_mesh->loadVertexArray(Box::CreateVertexArray({ 20, 20, 20 }));



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

		RenderCommand& command = _mesh->getRenderCommand();
		RenderState & state = command.state;

		command.shader = _shader.get();
		command.textureBlock.setTexture("environment", _texture.get());
		state.cullFace = RenderCullFace::Front;

	}

	SkyBox::~SkyBox()
	{
	}

	void SkyBox::render(Renderer * renderer)
	{
		_mesh->render(renderer);
	}

	glm::mat4& SkyBox::getModel()
	{
		return _mesh->getModel();
	}
	void SkyBox::setModel(glm::mat4& model)
	{
		_mesh->setModel(model);
	}



}
