
#pragma once

#include <Yare/App.hpp>
#include <Yare/FileSystem.hpp>
#include <Yare/Renderer/Shader.hpp>
#include <Yare/Renderer/Platform.hpp>
#include <Yare/Renderer/Texture.hpp>
#include <Yare/Renderer/VertexArray.hpp>
#include <Yare/Renderer/Primitives.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
//YARE_PROJECT_DIR defined by cmake
#define YARE_ASSET(path) YARE_PROJECT_DIR "/Assets/" path


using namespace yare;
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class SkySphere
{
public:


	SkySphere()
	{
		/*
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
		*/
		//use sphere
		std::vector<SphereVertex> vertices;
		std::vector<unsigned int> indices;
		CreateSphere(vertices, indices, 20, 20);

		_vertexArray.reset(VertexArray::Create());
		_vertexArray->bind();

		BufferLayout vertexLayout = {
			{BufferElementType::Float3, "position"},
			{BufferElementType::Float3, "normal"},
			{BufferElementType::Float2, "uv"},
		};
		VertexBuffer* vertexBuffer = VertexBuffer::Create(vertexLayout);
		//vertexBuffer->load(&vertices[0], sizeof(vertices));
		vertexBuffer->load(&vertices[0], vertices.size() * sizeof(vertices[0]));

		IndexBuffer* indexBuffer = IndexBuffer::Create();
		//indexBuffer->load(&indices[0], sizeof(indices));
		indexBuffer->load(&indices[0], indices.size() * sizeof(indices[0]));

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



		TexturePixels pixels;
		TextureFormat format;
		Texture::ReadFile(YARE_ASSET("Images/skybox.png") , pixels);
		
		int faceWidth; int faceHeight;
		faceWidth  = pixels.width / 4 ;
		faceHeight = pixels.height / 3;


		//right=0, left, up, down, back, front
		
		_texture.reset(Texture::Create(TextureType::CubeMap));
		TextureRegion region;
		region.width = faceWidth;
		region.height = faceHeight;

		
		//load right face
		region.xoffset = faceWidth * 2;
		region.yoffset = faceHeight * 1;
		Texture::ReadRegion(pixels, _faces[0], region);
		_texture->load(_faces[0], TextureFace::Right);
		
		//load left face
		region.xoffset = faceWidth * 0;
		region.yoffset = faceHeight * 1;
		Texture::ReadRegion(pixels, _faces[1], region);
		_texture->load(_faces[1], TextureFace::Left);

		//load up face
		region.xoffset = faceWidth * 1;
		region.yoffset = faceHeight * 0;
		Texture::ReadRegion(pixels, _faces[2], region);
		_texture->load(_faces[2], TextureFace::Top);

		//load down face
		region.xoffset = faceWidth * 1;
		region.yoffset = faceHeight * 2;
		Texture::ReadRegion(pixels, _faces[3], region);
		_texture->load(_faces[3], TextureFace::Bottom);


		//load front face
		region.xoffset = faceWidth * 1;
		region.yoffset = faceHeight * 1;
		Texture::ReadRegion(pixels, _faces[4], region);
		_texture->load(_faces[4], TextureFace::Front);

		//load back face
		region.xoffset = faceWidth * 3;
		region.yoffset = faceHeight * 1;
		Texture::ReadRegion(pixels, _faces[5], region);
		_texture->load(_faces[5], TextureFace::Back);


		_texture->generateMipMaps();

	}

	~SkySphere()
	{
	}

	void render(const glm::mat4 & projection, const glm::mat4& view)
	{
		_shader->bind();
		
		_shader->setUniform("projection", projection);
		_shader->setUniform("model", _model);
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
	
	glm::mat4 & getModel()
	{
		return _model;
	}
	void setModel(glm::mat4 & model)
	{
		_model = model;
	}

private:
	std::unique_ptr<VertexArray>  _vertexArray;
	std::unique_ptr<Shader > _shader;
	std::unique_ptr<Texture> _texture;
	TexturePixels _faces[6];
	glm::mat4 _model;
	
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
  glm::mat4 model = glm::mat4(1.0); 
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  std::shared_ptr<VertexArray> vertexArray;
  std::shared_ptr<Texture> texture;
  std::unique_ptr<SkySphere> _skybox;

};
