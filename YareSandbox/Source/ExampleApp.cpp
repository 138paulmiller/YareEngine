
#include "ExampleApp.hpp"
#include <Yare/Geometry/Box.hpp>
#include <Yare/Geometry/Sphere.hpp>
#include <Yare/Graphics/Materials/PhongMaterial.hpp>
#include <Yare/Graphics/Materials/FlatMaterial.hpp>
#include <Yare/AssetManager.hpp>
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

void CreateHeightMap(
	int xsize, int ysize, 
	std::vector<VertexType> & vertices,
	std::vector<unsigned int> & indices)
{
	BufferLayout vertexLayout = {

			{BufferElementType::Float3, "position"},
			{BufferElementType::Float3, "normal"},
			{BufferElementType::Float2, "uv"},
	};
	vertices.resize(xsize * ysize);
	indices.resize(xsize * ysize * 6);

	for (int y = 0; y <= ysize; ++y)
		for (int x = 0; x <= xsize; ++x) {
			float xx = (x - xsize / 2.0) / xsize * 10;
			float yy = (y - ysize / 2.0) / ysize * 10;


			vertices.push_back(getHeightMap({ xx, yy }, { x / (float)xsize, y / (float)ysize }));
		}

	for (int y = 0; y < ysize; ++y)
		for (int x = 0; x < xsize; ++x) {
			indices.push_back((x + 0) + (xsize + 1) * (y + 0));
			indices.push_back((x + 1) + (xsize + 1) * (y + 0));
			indices.push_back((x + 1) + (xsize + 1) * (y + 1));

			indices.push_back((x + 1) + (xsize + 1) * (y + 1));
			indices.push_back((x + 0) + (xsize + 1) * (y + 1));
			indices.push_back((x + 0) + (xsize + 1) * (y + 0));
		}
	std::cout << "vertices=" << vertices.size() << std::endl;
	std::cout << "index=" << indices.size() << std::endl;

}

PhongMesh::PhongMesh()
{
	//Default render data config
	RenderData& data = Mesh::renderData;
	data.shader = AssetManager::GetInstance().get<Shader>("Shader_Phong");
	data.mode = RenderMode::IndexedMesh;
	data.lighting = RenderLighting::Phong;
	data.primitive = RenderPrimitive::Triangles;
	data.state.cullFace = RenderCullFace::Back;
	data.state.winding = RenderWinding::Clockwise;
	data.state.depthFunc = RenderTestFunc::Less;
	data.state.stencilFunc = RenderTestFunc::Less;
}

void PhongMesh::preRender()
{
	
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

	//Load default engine assets
	AssetManager::GetInstance().loadEngineContent();

	{ //phong mesh
		_phongMesh.reset(new PhongMesh());
		_phongMesh->loadVertexArray(geometry::Box::CreateVertexArray({ 0.5,0.5,0.5 }));

		PhongMaterial *material = new PhongMaterial();

		Texture* diffuse = AssetManager::GetInstance().get<Texture>("Image_Container_Diffuse");
		Texture * specular = AssetManager::GetInstance().get<Texture>("Image_Container_Specular");

		diffuse->update(TextureWrap::Repeat);
		specular->update(TextureWrap::Repeat);

		material->setDiffuseTexture(diffuse);
		material->setSpecularTexture(specular);
		material->setShininess(2);

		_phongMesh->setMaterial(material);
	}
	//Set  up Sky box
	{ 
		_skybox.reset(new SkyBox());
		Texture* skyboxTexture = AssetManager::GetInstance().get<Cubemap>("Image_SkyBox");
		_skybox->setCubemap(skyboxTexture);
	}
	{

		
		_pointLight.reset(new PointLight());
		_pointLight->setAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
		_pointLight->setDiffuse(glm::vec3(0.54f, 0.2f, 0.2f));
		_pointLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		_pointLight->setAttenuation(glm::vec3(1, 0.40, 0.04 ));
	}
	{
		_pointLightMesh.reset(new Mesh());
		_pointLightMesh->loadVertexArray(geometry::Box::CreateVertexArray({ 0.25,0.25,0.25 }));

		FlatMaterial *material = new FlatMaterial();
		material->setBase(glm::vec3(1, 0, 0));
		_pointLightMesh->setMaterial(material);
		RenderData& data = _pointLightMesh->renderData;
		data.shader = AssetManager::GetInstance().get<Shader>("Shader_Flat");
		data.mode = RenderMode::IndexedMesh;
		data.lighting = RenderLighting::Flat;
		data.primitive = RenderPrimitive::Triangles;
		data.state.cullFace = RenderCullFace::Back;
		data.state.winding = RenderWinding::Clockwise;
		data.state.depthFunc = RenderTestFunc::Less;
		data.state.stencilFunc = RenderTestFunc::Less;
	}
	//Set up the scene
	_scene.setCamera(&_camera);

	_scene.add("PhongMesh", _phongMesh.get());
	_scene.add("Skybox1", _skybox.get());
	_scene.add("PointLightMesh", _pointLightMesh.get());
	_scene.getLights().setPointLight("PointLight0", _pointLight.get());
}


void ExampleApp::onExit()
{
}

void ExampleApp::onRender() {


	float t = getTime();
	// set matrix : projection + view
	_projection = glm::perspective(45.0f,getWindowRatio(), 0.1f, 200.0f);
	
	_camera.setProjection(_projection);
	_camera.setPosition({ 0, 0, 2 });

	glm::vec3 front;
	float yaw = t*10, pitch = 0;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	
	_camera.setForward({0,0,-1});
	// clear
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////// Update State ////////////////
 	 _model = glm::translate(_camera.getPosition());
	 _model *= glm::scale(glm::vec3( 10,10,10 ));
	_skybox->setModel(_model);
	
	_phongMesh->setModel(glm::translate(glm::vec3( 0,0,0)));

	_pointLight->setPosition({ cos(t),  0.2f,  sin(t) * 2.0f });
	
	_pointLightMesh->setModel(glm::translate( _pointLight->getPosition()));

	//_phongMesh->renderData.state.wireframe = true;

	//Submit Scene to be drawn - TODO - SceneRenderer will manage /sort/ cull this process of drawing
	_scene.render(App::getRenderer());
	
} 