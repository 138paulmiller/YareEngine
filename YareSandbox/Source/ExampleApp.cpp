
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
	floorTransform *= glm::translate(glm::vec3({ 0,-3,-5 }));
	floorTransform *= glm::scale(glm::vec3(10, 0.120, 5.0));
	_floorMesh->setTransform(floorTransform);
	//--------------------------------- Init boxes -------------------------------------------
	_boxMeshes.resize(BOX_COUNT);
	for (int i = 0; i < BOX_COUNT; i++)
	{ //phong mesh
		_boxMeshes[i].reset(new Mesh());
		_boxMeshes[i]->loadVertexArray(geometry::Box::CreateVertexArray({ 1,1,1 }));
		_boxMeshes[i]->setMaterial(_phongMaterial.get());
	}


	//--------------------------------- Init lights and corresponding meshes -------------------------------------------
	_directionalLights.resize(LIGHT_COUNT);
	_directionalLightMeshes.resize(LIGHT_COUNT);
	float radius = 8;
	float freq = 3.1514 / 50;
	float phase= 3.1514 / 9;
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		glm::vec3 position = {
			radius * sin(i * freq + phase ) * cos(i * freq + phase),
			radius * sin(i * freq + phase ) * sin(i * freq + phase) ,
			radius * cos(i * freq + phase ) 
		};
		//light 0
		_directionalLights[i].reset(new DirectionalLight());
		_directionalLights[i]->setAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
		_directionalLights[i]->setDiffuse(glm::vec3(0.254f, 0.252f, 0.252f));
		_directionalLights[i]->setSpecular(glm::vec3(0.15f, 0.15f, 0.15f));
		_directionalLights[i]->setCastShadow(true);
		_directionalLights[i]->setPosition(position);
		_directionalLights[i]->setDirection(glm::normalize(glm::vec3(0) - _directionalLights[i]->getPosition()));
	

		_directionalLightMeshes[i].reset(new Mesh());
		_directionalLightMeshes[i]->loadVertexArray(geometry::Sphere::CreateVertexArray(0.05, 50, 50));
		_directionalLightMeshes[i]->setTransform(glm::translate(_directionalLights[i]->getPosition()));
		_directionalLightMeshes[i]->setMaterial(_flatMaterial.get());
	}
	


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
		_scene.getLights().setDirectionalLight("DirectionalLight_" + std::to_string(index), directionalLight.get());
		index++;
	}
	index = 0;
	for (const std::unique_ptr<Mesh>& directionalLight : _directionalLightMeshes)
	{
		_scene.add("DirectionalLightMesh_" + std::to_string(index), directionalLight.get());
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
	_camera->setPosition({ 5, 5, 15 });
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
	glm::mat4 scale, rotate, translate;
	float radians= getTime() * (speed);
	float i = 0;
	float radius = 5;
	float freq = 3.1514 / BOX_COUNT;
	//update box 1
	for (const std::unique_ptr<Mesh>& boxMesh : _boxMeshes)
	{
		glm::vec3 position = {
			radius * sin(i * freq) * cos(i * freq),
			radius * sin(i * freq) * sin(i * freq) ,
			radius * cos(i * freq)
		};

		scale = glm::scale(glm::vec3(0.13, 0.13 ,0.13));
		translate = glm::translate(position);
		rotate = glm::rotate(radians, glm::vec3(sin(i),cos(i),sin(i)));
		boxMesh->setTransform(translate * scale * rotate);
		i++;
	}
	

}


