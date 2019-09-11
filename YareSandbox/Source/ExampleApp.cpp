
#include "ExampleApp.hpp"


#include <iostream>
#include <vector>


struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

float heightMap(const glm::vec2 position) {
  return  2 * sin(position.x) * sin(position.y);
}

VertexType getHeightMap(const glm::vec2 position, const glm::vec2 uv) {
  const glm::vec2 dx(1.0, 0.0);
  const glm::vec2 dy(0.0, 1.0);

  VertexType v;
  float h = heightMap(position);
  float hx = 100.f * (heightMap(position + 0.01f * dx) - h);
  float hy = 100.f * (heightMap(position + 0.01f * dy) - h);

  v.position = glm::vec3(position.x, h, position.y);
  v.normal = glm::normalize(glm::vec3(-hx, -hy, 1.0));

  float c = sin(h * 5.f) * 0.5 + 0.5;
  v.uv = uv;
  return v;
}


ExampleApp::ExampleApp() 
{
}


void ExampleApp::onEnter()
{
	_skySphere.reset(new SkyBox());


	////////////////////////////// Demo Heightmap ////////////////////////////////
	std::string vertSource, fragSource;
	FileSystem::readFile(YARE_ASSET("Shaders/phong.vert") ,vertSource );
	FileSystem::readFile(YARE_ASSET("Shaders/phong.frag") , fragSource );

	_simpleShader = std::shared_ptr<Shader>(Shader::Create());

	_simpleShader->compile(vertSource, fragSource);

	// creation of the mesh ------------------------------------------------------
	std::vector<VertexType> vertices;
	std::vector<unsigned int> indices;
  

	for (int y = 0; y <= _size; ++y)
		for (int x = 0; x <= _size; ++x) {
			float xx = (x - _size / 2) * 0.1f;
			float yy = (y - _size / 2) * 0.1f;
			vertices.push_back(getHeightMap({ xx, yy }, { x/(float)_size, y / (float)_size }));
	}

	for (int y = 0; y < _size; ++y)
	for (int x = 0; x < _size; ++x) {
		indices.push_back((x + 0) + (_size + 1) * (y + 0));
		indices.push_back((x + 1) + (_size + 1) * (y + 0));
		indices.push_back((x + 1) + (_size + 1) * (y + 1));

		indices.push_back((x + 1) + (_size + 1) * (y + 1));
		indices.push_back((x + 0) + (_size + 1) * (y + 1));
		indices.push_back((x + 0) + (_size + 1) * (y + 0));
	}

	std::cout << "vertices=" << vertices.size() << std::endl;
	std::cout << "index=" << indices.size() << std::endl;

	// creation of the vertex array buffer----------------------------------------
	//1 . Create VAO, then VBO, then IBO
	_vertexArray = std::shared_ptr< VertexArray>(VertexArray::Create());
	_vertexArray->bind();

	BufferLayout vertexLayout = {

		{BufferElementType::Float3, "position"},
		{BufferElementType::Float3, "normal"},
		{BufferElementType::Float2, "uv"},
	};
	VertexBuffer * vertexBuffer = VertexBuffer::Create(vertexLayout);
	vertexBuffer->load(&vertices[0], vertices.size() * sizeof(VertexType));
	
	
	IndexBuffer* indexBuffer = IndexBuffer::Create();
	indexBuffer->load(&indices[0], indices.size() * sizeof(unsigned int ));
	

	_vertexArray->addVertexBuffer(vertexBuffer);
	_vertexArray->setIndexBuffer(indexBuffer);

	_vertexArray->unbind();

	// Load some textures ----------------------------------------

	TexturePixels pixelsRegion;
	TexturePixels pixels;

	_textureDiffuse.reset(Texture::Create());
	_textureSpecular.reset(Texture::Create());

	Texture::ReadFile(YARE_ASSET("Images/container_diffuse.png"), pixels);
	_textureDiffuse->load(pixels);
	_textureDiffuse->generateMipMaps();

	Texture::ReadFile(YARE_ASSET("Images/container_specular.png"), pixels);
	_textureSpecular->load(pixels);
	_textureSpecular->generateMipMaps();
			

}


void ExampleApp::onExit()
{
}

void ExampleApp::onRender(Renderer* renderer) {


	float t = getTime();
	// set matrix : projection + view
	_projection = glm::perspective(45.0f,getWindowRatio(), 0.1f, 100.f);
	
	_camera.setProjection(_projection);
	_camera.setPosition({ 10.0 * sin(t), 0, 10.0 * cos(t) });
	_camera.lookAt({ 0,0,0 });
	// clear
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_command.uniformBlock.setUniform("projection", _camera.getProjection());
	_command.uniformBlock.setUniform("view", _camera.getView());
	_command.uniformBlock.setUniform("model", glm::mat4(1));
	_command.uniformBlock.setUniform("material.shininess", 120.0f);
	_command.vertexArray = _vertexArray.get();
	_command.shader = _simpleShader.get();
	_command.textureBlock.setTexture("material.diffuse", _textureDiffuse.get());
	_command.textureBlock.setTexture("material.specular", _textureSpecular.get());
	_command.mode = RenderMode::IndexedMesh;
	_command.primitive = RenderPrimitive::Triangles;
	_command.state = {
		RenderCullFace::Back,
		RenderWinding::Clockwise,
		RenderTestFunc::Less,
		RenderTestFunc::Less,
	};

	renderer->beginScene(&_camera);
	
	renderer->submit(&_command);

	renderer->endScene();

	//simpleShader->bind();
	//
	//simpleShader->setUniform("projection", camera.getProjection());
	//simpleShader->setUniform("view", camera.getView());
	//simpleShader->setUniform("model", glm::mat4(1));
  	//
	//texture->bind(0);
	//vertexArray->bind();
	//glDrawElements(GL_TRIANGLES,         // mode
	//	vertexArray->getIndexBuffer()->getIndexCount() ,  // count
	//				GL_UNSIGNED_INT,      // type
	//				NULL                  // element array buffer offset
	//);
	//
	//vertexArray->unbind();
	//texture->unbind();
	//simpleShader->unbind();
  

  // Rendering API
  renderer->beginScene(&_camera);
	  _model = glm::translate(_camera.getPosition());
	  _skySphere->setModel(_model);
	  _skySphere->render(renderer);

  renderer->endScene();
} 