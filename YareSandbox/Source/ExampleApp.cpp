
#include "ExampleApp.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include <iostream>
#include <vector>


#define SHADER_DIR YARE_PROJECT_DIR "/Assets/Shaders/"

struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

float heightMap(const glm::vec2 position) {
  return 2.0 * sin(position.x) * sin(position.y);
}

VertexType getHeightMap(const glm::vec2 position, const glm::vec2 uv) {
  const glm::vec2 dx(1.0, 0.0);
  const glm::vec2 dy(0.0, 1.0);

  VertexType v;
  float h = heightMap(position);
  float hx = 100.f * (heightMap(position + 0.01f * dx) - h);
  float hy = 100.f * (heightMap(position + 0.01f * dy) - h);

  v.position = glm::vec3(position, h);
  v.normal = glm::normalize(glm::vec3(-hx, -hy, 1.0));

  float c = sin(h * 5.f) * 0.5 + 0.5;
  v.uv = uv;
  return v;
}

ExampleApp::ExampleApp(){

	Platform::Current().setRenderer(PlatformRenderer::OpenGL);

	std::string vertSource, fragSource;
	FileSystem::readFile( SHADER_DIR "/shader.vert",vertSource );
	FileSystem::readFile(SHADER_DIR "/shader.frag", fragSource );

	simpleShader = std::shared_ptr<Shader>(Shader::Create());

	simpleShader->compile(vertSource, fragSource);

	// creation of the mesh ------------------------------------------------------
	std::vector<VertexType> vertices;
	std::vector<unsigned int> indices;
  

	for (int y = 0; y <= size; ++y)
		for (int x = 0; x <= size; ++x) {
			float xx = (x - size / 2) * 0.1f;
			float yy = (y - size / 2) * 0.1f;
			vertices.push_back(getHeightMap({ xx, yy }, { x/(float)size, y / (float)size }));
	}

	for (int y = 0; y < size; ++y)
	for (int x = 0; x < size; ++x) {
		indices.push_back((x + 0) + (size + 1) * (y + 0));
		indices.push_back((x + 1) + (size + 1) * (y + 0));
		indices.push_back((x + 1) + (size + 1) * (y + 1));

		indices.push_back((x + 1) + (size + 1) * (y + 1));
		indices.push_back((x + 0) + (size + 1) * (y + 1));
		indices.push_back((x + 0) + (size + 1) * (y + 0));
	}

	std::cout << "vertices=" << vertices.size() << std::endl;
	std::cout << "index=" << indices.size() << std::endl;

	// creation of the vertex array buffer----------------------------------------
	//1 . Create VAO, then VBO, then IBO
	vertexArray = std::shared_ptr< VertexArray>(VertexArray::Create());
	vertexArray->bind();

	BufferLayout vertexLayout = {

		{BufferElementType::Float3, "position"},
		{BufferElementType::Float3, "normal"},
		{BufferElementType::Float2, "uv"},
	};
	VertexBuffer * vertexBuffer = VertexBuffer::Create(vertexLayout);
	vertexBuffer->load(&vertices[0], vertices.size() * sizeof(VertexType));
	
	
	IndexBuffer* indexBuffer = IndexBuffer::Create();
	indexBuffer->load(&indices[0], indices.size() );
	

	vertexArray->addVertexBuffer(vertexBuffer);
	vertexArray->setIndexBuffer(indexBuffer);

	vertexArray->unbind();

	// Load some textures ----------------------------------------

	unsigned char  * data;
	int width; int height;
	TextureFormat format;

	Texture::ReadFile("Assets/Models/alliance.png", &data, width, height, format);
	texture = std::shared_ptr < Texture>(Texture::Create() );
	texture->load(format, data, 0, width, height);
	delete data;


}
#include <Yare/Renderer/OpenGL/OpenGLError.hpp>
void ExampleApp::onRender() {
  

  float t = getTime();
  // set matrix : projection + view
  projection = glm::perspective(float(2.0 * atan(getHeight() / 1920.f)),
                                getWindowRatio(), 0.1f, 100.f);
  view = glm::lookAt(glm::vec3(20.0 * sin(t), 20.0 * cos(t), 20.0),
                     glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

  // clear
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  simpleShader->bind();

  
  simpleShader->setUniform("projection", projection);
  simpleShader->setUniform("view", view);

  texture->bind(0);
  vertexArray->bind();
  glDrawElements(GL_TRIANGLES,         // mode
	  vertexArray->getIndexBuffer()->getIndexCount() ,  // count
                 GL_UNSIGNED_INT,      // type
                 NULL                  // element array buffer offset
  );

  texture->unbind();
  vertexArray->unbind();

  simpleShader->unbind();
}
