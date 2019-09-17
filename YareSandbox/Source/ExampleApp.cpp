
#include "ExampleApp.hpp"
#include <Yare/Geometry/Box.hpp>
#include <Yare/Geometry/Sphere.hpp>
#include <Yare/Graphics/Materials/PhongMaterial.hpp>

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
PhongMesh::PhongMesh()
{
	std::string vertSource, fragSource;
	FileSystem::readFile(YARE_ASSET("Shaders/phong.vert"), vertSource);
	FileSystem::readFile(YARE_ASSET("Shaders/phong.frag"), fragSource);

	_phongShader.reset(Shader::Create());

	_phongShader->compile(vertSource, fragSource);

}

void PhongMesh::preRender()
{
	RenderData& command = Mesh::renderData;
	command.shader = _phongShader.get();
	command.mode = RenderMode::IndexedMesh;
	command.primitive = RenderPrimitive::Triangles;
	command.state.cullFace = RenderCullFace::Back;

	command.state.winding= RenderWinding::Clockwise;
	command.state.depthFunc = RenderTestFunc::Less;
	command.state.stencilFunc = RenderTestFunc::Less;

	Mesh::preRender();
}

void PhongMesh::postRender()
{
	Mesh::postRender();
}




ExampleApp::ExampleApp() 
{
}


void ExampleApp::onEnter()
{
	_skySphere.reset(new SkyBox());


	////////////////////////////// Demo Heightmap ////////////////////////////////
	_phongMesh.reset(new PhongMesh());

	// creation of the mesh ------------------------------------------------------
	BufferLayout vertexLayout = {

			{BufferElementType::Float3, "position"},
			{BufferElementType::Float3, "normal"},
			{BufferElementType::Float2, "uv"},
	};
	std::vector<VertexType> vertices;
	std::vector<unsigned int> indices;
  
	for (int y = 0; y <= _size; ++y)
		for (int x = 0; x <= _size; ++x) {
			float xx = (x - _size / 2.0) / _size * 10;
			float yy = (y - _size / 2.0) / _size * 10;


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

	_phongMesh->loadVertices(vertices, vertexLayout);
	_phongMesh->loadIndices(indices);
	
	PhongMaterial *material = new PhongMaterial();

	material->setDiffuseTexture(Texture::CreateFromFile(YARE_ASSET("Images/container_diffuse.png")));
	material->setSpecularTexture(Texture::CreateFromFile(YARE_ASSET("Images/container_specular.png")));
	material->setShininess(128);
	
	_phongMesh->setMaterial(material);
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

	////////// Update State ////////////////
 	 _model = glm::translate(_camera.getPosition());
	_skySphere->setModel(_model);

	_model = glm::scale(glm::vec3({ 10,10,10 }));

	//_phongMesh->setModel(_model);

	_phongMesh->renderData.state.wireframe = true;
	//Submit Scene to be drawn 

	renderer->beginScene(&_camera);
		renderer->submit(_phongMesh.get());
		renderer->submit(_skySphere.get());
	renderer->endScene();
} 