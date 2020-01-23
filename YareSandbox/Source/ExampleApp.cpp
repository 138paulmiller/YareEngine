
#include "ExampleApp.hpp"
#include <Yare/Geometry/Box.hpp>
#include <Yare/Geometry/Quad.hpp>
#include <Yare/Geometry/Sphere.hpp>
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







ExampleApp::ExampleApp() 
{
}

//////////////////////////////////// Enter ////////////////////////////////////////////////////////
void ExampleApp::onEnter()
{

	//--------------------------------- Init Textures -------------------------------------------

	//Create an instance of the phong material. To be shared among meshes

	Texture* diffuse = AssetManager::GetInstance().get<Texture>("container_diffuse");
	diffuse->update(TextureWrap::Repeat);
	
	Texture * specular = AssetManager::GetInstance().get<Texture>("container_specular");
	specular->update(TextureWrap::Repeat);

	Texture* oceanImage= AssetManager::GetInstance().get<Texture>("ocean");
	oceanImage->update(TextureWrap::Repeat);


	//All materials should set all textures as empty to default. Move this to texture class to get empty texture
	Texture* empty = Texture::Create();
	TexturePixels pixels = { 0, 0, 0};
	pixels.height = pixels.height = 1;
	pixels.format = TextureFormat::RGB8;
	empty->update(TextureWrap::Repeat);
	empty->load(pixels);
	empty->generateMipMaps();


	//--------------------------------- Init materials -------------------------------------------
	_phongMaterial.reset(new PhongMaterial());
	_phongMaterial->setDiffuseTexture(diffuse);
	_phongMaterial->setSpecularTexture(specular);
	//_phongMaterial->setEmissiveTexture(empty);
	_phongMaterial->setShininess(2);

	//Create an instance of the flat material. To be shared among meshes
	_flatMaterial.reset(new FlatMaterial());
	_flatMaterial->setBase(glm::vec3(1, 0, 0));

	_floorMaterial.reset(new PhongMaterial());	
	_floorMaterial->setDiffuseTexture(oceanImage);

	// --------------- Init Floor ---------------------------------------------------------------
	_floorMesh.reset(new Mesh());
	_floorMesh->setMaterial(_floorMaterial.get());
	_floorMesh->loadVertexArray(geometry::Box::CreateVertexArray({ 1, 1, 1 }));

	glm::mat4 floorTransform = glm::rotate(-3.14159f / 6.f, glm::vec3(0, 0, 1));
	floorTransform *= glm::translate(glm::vec3({ 0,-3,0 }));
	floorTransform *= glm::scale(glm::vec3(4, 0.120, 5.0));
	_floorMesh->setTransform(floorTransform);
	//--------------------------------- Init boxes -------------------------------------------
	_boxMeshes.resize(3);
	for (int i = 0; i < 3; i++)
	{ //phong mesh
		_boxMeshes[i].reset(new Mesh());
		_boxMeshes[i]->loadVertexArray(geometry::Box::CreateVertexArray({ 1,1,1 }));
		_boxMeshes[i]->setMaterial(_phongMaterial.get());
	}


	//--------------------------------- Init lights and corresponding meshes -------------------------------------------
	_directionalLights.resize(2);
	_directionalLightMeshes.resize(2);
	//light 0
	_directionalLights[0].reset(new DirectionalLight());
	_directionalLights[0]->setAmbient(glm::vec3(0.15f, 0.15f, 0.15f));
	_directionalLights[0]->setDiffuse(glm::vec3(0.654f, 0.652f, 0.652f));
	_directionalLights[0]->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	_directionalLights[0]->setCastShadow(true);
	_directionalLights[0]->setPosition(glm::vec3( 7, 5, 0 ));
	_directionalLights[0]->setDirection(glm::normalize(glm::vec3(0) - _directionalLights[0]->getPosition()));
	

	_directionalLightMeshes[0].reset(new Mesh());
	_directionalLightMeshes[0]->loadVertexArray(geometry::Box::CreateVertexArray({ 0.15,0.15,0.15 }));
	_directionalLightMeshes[0]->setTransform(glm::translate(_directionalLights[0]->getPosition()));
	_directionalLightMeshes[0]->setMaterial(_flatMaterial.get());
	
	//light 1

	_directionalLights[1].reset(new DirectionalLight());
	_directionalLights[1]->setAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
	_directionalLights[1]->setDiffuse(glm::vec3(0.654f, 0.652f, 0.652f));
	_directionalLights[1]->setSpecular(glm::vec3(0.8f, 0.8f, 0.8f));
	_directionalLights[1]->setCastShadow(true);
	_directionalLights[1]->setPosition(glm::vec3(-1, 6, 0));
	_directionalLights[1]->setDirection(glm::normalize(glm::vec3(0) - _directionalLights[1]->getPosition()));


	_directionalLightMeshes[1].reset(new Mesh());
	_directionalLightMeshes[1]->loadVertexArray(geometry::Box::CreateVertexArray({ 0.15,0.15,0.15 }));
	_directionalLightMeshes[1]->setTransform(glm::translate(_directionalLights[1]->getPosition()));
	_directionalLightMeshes[1]->setMaterial(_flatMaterial.get());



	//--------------------------------- Set  up Sky box---------------------------------
	_skybox.reset(new SkyBox());
	Texture* skyboxTexture = AssetManager::GetInstance().get<Cubemap>("skybox_cubemap");
	_skybox->setCubemap(skyboxTexture);


	//--------------------------------------- Set up the scene ---------------------------------
	_scene.add("Skybox", _skybox.get());
	_scene.add("FloorMesh", _floorMesh.get());
	int index = 0;
	for (const std::unique_ptr<Mesh>& boxMesh : _boxMeshes)
	{
		_scene.add("PhongMesh_" + std::to_string(index), boxMesh.get());
		index++;
	}

	index = 0;
	for (const std::unique_ptr<DirectionalLight>& directionalLight : _directionalLights)
	{
		_scene.getLights().setDirectionalLight("DirectionalLight_" + std::to_string(index), _directionalLights[index].get());
		index++;
	}
	index = 0;
	for (const std::unique_ptr<Mesh>& directionalLight : _directionalLightMeshes)
	{
		_scene.add("DirectionalLightMesh_" + std::to_string(index), _directionalLightMeshes[index].get());
		index++;
	}


	
}


void ExampleApp::onExit()
{
	
}

void ExampleApp::onRender() {

	float time = getTime();

	if (_elapsedTime > 1.0) //for every one second.
	{
		std::cout << "\nFPS = " << _frames / (_elapsedTime);
		_elapsedTime = 0;
		_frames = 0;
	}
	else
	{
		_elapsedTime += getDeltaTime();
		_frames++;
	}
	// set matrix : projection + view
	
	updateCamera();
	updateSkybox();
	updateBoxes();

	//Submit Scene to be drawn - TODO - SceneRenderer will manage /sort/ cull this process of drawing
	_scene.setCamera(_camera.get());
	_scene.render(App::getRenderer());
} 

void ExampleApp::updateCamera()
{
	_camera.reset(new PerspectiveCamera(45.0f, getWindowRatio(), 1.0f, 200.0f));
	_camera->setPosition({ 0, 0, 15 });
	_camera->setForward(glm::normalize(glm::vec3(0) - _camera->getPosition()));

}

void ExampleApp::updateSkybox()
{
	glm::mat4 transform = glm::translate(_camera->getPosition());
	transform *= glm::scale(glm::vec3(100, 100, 100));
	_skybox->setTransform(transform);
}

void ExampleApp::updateBoxes()
{
	float speed = 0.25;
	glm::mat4 transform = glm::mat4(1);
	float radians= getTime() * (speed);
	glm::vec3 position;
	//update box 1
	position = { 0,0,0 };
//	transform = glm::scale(glm::vec3(0.3, 0.3, 0.3));
	transform = glm::translate(transform, position);
	transform *= glm::rotate(transform, radians, glm::vec3(0,1,0));
	_boxMeshes[0]->setTransform(transform);
	
	//update box 2

	position = { 3,1,0 };
//	transform = glm::scale(glm::vec3(0.1, 0.1, 0.1 ));
	transform = glm::translate(position);
	transform *= glm::rotate(radians, glm::vec3( 1.f,1.f,0.f));
	_boxMeshes[1]->setTransform(transform);


	position = { 1,-3,0 };
	//	transform = glm::scale(glm::vec3(0.1, 0.1, 0.1 ));
	transform = glm::translate(position);
	_boxMeshes[2]->setTransform(transform);


}


