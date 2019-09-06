
#ifndef OPENGL_CMAKE_SKELETON_MYApp
#define OPENGL_CMAKE_SKELETON_MYApp

#include <Yare/App.hpp>
#include <Yare/FileSystem.hpp>
#include <Yare/Renderer/Shader.hpp>
#include <Yare/Renderer/Platform.hpp>
#include <Yare/Renderer/Texture.hpp>
#include <Yare/Renderer/VertexArray.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

//YARE_PROJECT_DIR defined by cmake
#define YARE_ASSET(path) YARE_PROJECT_DIR "/Assets/" path


using namespace yare;


class SkyBox
{
public:
	SkyBox()
	{
		//Create Cube VertexArray
		glm::vec3 vertices[8] = {
			//front
			{-0.5, -0.5,  0.5 },
			{ 0.5, -0.5,  0.5 },
			{ 0.5,  0.5,  0.5 },
			{-0.5,  0.5,  0.5 },
			// back
			{ -0.5, -0.5, -0.5 },
			{  0.5, -0.5, -0.5 },
			{  0.5,  0.5, -0.5 },
			{ -0.5,  0.5, -0.5 }
		};
		unsigned int indices[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		};

		_vertexArray.reset(VertexArray::Create());
		_vertexArray->bind();

		BufferLayout vertexLayout = {
			{BufferElementType::Float3, "position"},
		};
		VertexBuffer* vertexBuffer = VertexBuffer::Create(vertexLayout);
		vertexBuffer->load(&vertices[0], sizeof(vertices));

		IndexBuffer* indexBuffer = IndexBuffer::Create();
		indexBuffer->load(&indices[0], sizeof(indices));

		_vertexArray->addVertexBuffer(vertexBuffer);
		_vertexArray->setIndexBuffer(indexBuffer);
		_vertexArray->unbind();

		//Create the Skybox Shader
		std::string vertSource, fragSource;
		FileSystem::readFile(YARE_ASSET("Shaders/skybox.vert") , vertSource);
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

		unsigned char* data;
		int width; int height;
		int level = 0;
		int faceWidth; int faceHeight;
		TextureFormat format;
		format = Texture::ReadFile(YARE_ASSET("Images/skybox.png") , &data, width, height);
		faceWidth  = width / 4 ;
		faceHeight = height / 3;

		_texture.reset(Texture::Create(TextureType::CubeMap));
		TextureRegion region;
		region.width = width;
		region.height = height;
		//Must load entire texture into each face. Then can read 


		//right, left, up, down, back, front
		region.width = faceWidth;
		region.height = faceHeight;
		//load right face
		region.yOffset = faceWidth * 2;
		region.xOffset = faceHeight * 1;
		_texture->load(data, width, height, format, region, TextureFace::Right);
		//load left face
		region.yOffset = faceWidth * 0;
		region.xOffset = faceHeight * 1;
		_texture->load(data, width, height, format, region, TextureFace::Left);
		//load up face
		region.yOffset = faceWidth * 1;
		region.xOffset = faceHeight * 0;
		_texture->load(data, width, height, format, region, TextureFace::Up);
		//load down face
		region.yOffset = faceWidth * 1;
		region.xOffset = faceHeight * 2;
		_texture->load(data, width, height, format, region, TextureFace::Down);
		//load back face
		region.yOffset = faceWidth * 3;
		region.xOffset = faceHeight * 1;
		_texture->load(data, width, height, format, region, TextureFace::Back);
		//load front face
		region.yOffset = faceWidth * 1;
		region.xOffset = faceHeight * 1;
		_texture->load(data, width, height, format, region, TextureFace::Front);


	}

	~SkyBox()
	{
	}

	void render(const glm::mat4 & projection, const glm::mat4& view)
	{
		_shader->bind();
		
		_shader->setUniform("projection", projection);
		_shader->setUniform("view", view);

			_texture->bind(0);
				_vertexArray->bind();
				glDrawElements(GL_TRIANGLES,         // mode
					_vertexArray->getIndexBuffer()->getIndexCount(),  // count
					GL_UNSIGNED_INT,      // type
					NULL                  // element array buffer offset
				);
				_vertexArray->unbind();
			_texture->unbind();
		_shader->unbind();
	}
private:
	std::unique_ptr<VertexArray>  _vertexArray;
	std::unique_ptr<Shader > _shader;
	std::unique_ptr<Texture> _texture;
	
};

class ExampleApp : public App {
 public:
  ExampleApp();

 protected:
  virtual void onRender() override;

 private:
  float time = 0.f;
  const int size = 100;

  // shader

  std::shared_ptr<Shader > simpleShader;

  // shader matrix uniform
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  std::shared_ptr<VertexArray> vertexArray;
  std::shared_ptr<Texture> texture;
  std::unique_ptr<SkyBox> _skybox;

};

#endif  // OPENGL_CMAKE_SKELETON_MYApp
